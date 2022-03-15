#include "data_structures.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

/* Initialization of head data structure. */
void head_init(head* arr, int tableSize, int dataSize, int codeSize) {
    arr->table = (symbol*)malloc_with_monitor(tableSize * sizeof(symbol));
    arr->data_image = (image*)malloc_with_monitor(dataSize * sizeof(image));
    arr->code_image = (image*)malloc_with_monitor(codeSize * sizeof(image));
    arr->tableUsed = arr->dataUsed = arr->codeUsed = 0;
    arr->tableSize = tableSize;
    arr->dataSize = dataSize;
    arr->codeSize = codeSize;
}

/* Initialization of symbol node. */
void symbol_init(symbol* node) {
    node->isExternal = False;
    node->isCode = False;
    node->isData = False;
    node->isEntry = False;
}

/* SYMBOLS */

/* Handles insertion into symbol table. */
void insert_symbol(head* arr, char* name, int value, int base, int offset, operation op) {
    int idx = arr->tableUsed;
    if (arr->tableUsed == arr->tableSize) {
        arr->tableSize *= 2;
        arr->table = (symbol*)realloc_with_monitor(arr->table, arr->tableSize * sizeof(symbol));
    }

    strcpy(arr->table[idx].symbol_name, name);
    arr->table[idx].value = value;
    arr->table[idx].base = base;
    arr->table[idx].offset = offset;

    symbol_init(arr->table + idx);

    if (op == DATA || op == STRING)
        arr->table[idx].isData = True;
    else if (op == ENTRY)
        arr->table[idx].isEntry = True;
    else if (op == EXTERNAL)
        arr->table[idx].isExternal = True;
    else /* Assume it is fine */
        arr->table[idx].isCode = True;

    arr->tableUsed++;
}

/* Inserts into symbol table when the line is a data line. */
void insert_data_symbol(head* arr, char* name, int value, operation op) {
    insert_symbol(arr, name, value, 0, 0, op);
}

/* Inserts into symbol table when the line is of type .extern. */
void insert_extern(head* arr, char* line, operation op) {
    delete_spaces(line);
    insert_symbol(arr, line, 0, 0, 0, op);
}

/* Inserts into symbol table when dealing with an instruction line. */
void insert_code_symbol(head* arr, char* name, int value, operation op) {
    insert_symbol(arr, name, value, getBase(value), getHist(value), op);
}

/* Frees the symbol table's memory allocation. */
void free_symbol_table(head* arr) {
    free(arr->table);
    arr->table = NULL;
    arr->tableUsed = arr->tableSize = 0;
}

/* DATA */

/* Handles insertion into data image. */
void insert_data_img(head* arr, unsigned int data, int line) {
    int idx = arr->dataUsed;
    if (arr->dataUsed == arr->dataSize) {
        arr->dataSize *= 2;
        arr->data_image = (image*)realloc_with_monitor(arr->data_image, arr->dataSize * sizeof(image));
    }
    arr->data_image[idx].line = line;
    arr->data_image[idx].bin.word.opcode = data;
    arr->data_image[idx].bin.word.attribute = A;

    arr->dataUsed++;
}
/* This function updates the Data Count in the Symbol Table and line number in data-image after first pass. */
void update_data_count(head* arr, int inst_count) {
    int idx;
    for (idx = 0; idx < arr->dataUsed; idx++)
        arr->data_image[idx].line += inst_count;

    /* Update symbol table and set hist & base. */
    for (idx = 0; idx < arr->tableUsed; idx++)
        if (arr->table[idx].isData) {
            arr->table[idx].value += inst_count;
            arr->table[idx].base = getBase(arr->table[idx].value);
            arr->table[idx].offset = getHist(arr->table[idx].value);
        }
}

/* Frees all memory allocations of data image. */
void free_data_image(head* arr) {
    free(arr->data_image);
    arr->data_image = NULL;
    arr->dataUsed = arr->dataSize = 0;
}

/* CODE */

/* Handles insertion into code image when given an opcode. */
void insert_code_opcode(head* arr, unsigned int attribute, unsigned int opcode, int line) {
    int idx = arr->codeUsed;
    if (arr->codeUsed == arr->codeSize) {
        arr->codeSize *= 2;
        arr->code_image = (image*)realloc_with_monitor(arr->code_image, arr->codeSize * sizeof(image)); /* Add monitor */
    }
    arr->code_image[idx].line = line;
    arr->code_image[idx].isExtern = False;
    arr->code_image[idx].bin.word.opcode = (1 << opcode);
    arr->code_image[idx].bin.word.attribute = attribute;
    arr->code_image[idx].toDecode = DONE;

    arr->codeUsed++;
}

/* Handles insertion into code image when given a "line_info" variable. */
void insert_code_instruction(head* arr, line_info* instruction, int line) {
    int idx = arr->codeUsed;
    if (arr->codeUsed == arr->codeSize) {
        arr->codeSize *= 2;
        arr->code_image = (image*)realloc_with_monitor(arr->code_image, arr->codeSize * sizeof(image)); /* Add monitor */
    }

    arr->code_image[idx].bin.instruction.attribute = A;
    arr->code_image[idx].bin.instruction.funct = instruction->funct;

    arr->code_image[idx].bin.instruction.dest_addr = instruction->dst_addr;
    arr->code_image[idx].bin.instruction.dest_reg = instruction->dst_reg;
    arr->code_image[idx].bin.instruction.src_addr = instruction->src_addr;
    arr->code_image[idx].bin.instruction.src_reg = instruction->src_reg;

    arr->code_image[idx].toDecode = DONE;
    arr->code_image[idx].line = line;
    arr->code_image[idx].isExtern = False;

    arr->codeUsed++;
}

/* Handles insertion into code image when given an immediate number. */
void insert_code_immediate(head* arr, int immediate, int attribute, int line, char* label, int status) {
    int idx = arr->codeUsed;
    if (arr->codeUsed == arr->codeSize) {
        arr->codeSize *= 2;
        arr->code_image = (image*)realloc_with_monitor(arr->code_image, arr->codeSize * sizeof(image)); /* Add monitor */
    }

    arr->code_image[idx].line = line;
    arr->code_image[idx].isExtern = False;
    arr->code_image[idx].toDecode = status;
    strcpy(arr->code_image[idx].label, label);

    arr->code_image[idx].bin.word.attribute = attribute;
    arr->code_image[idx].bin.word.opcode = immediate;

    arr->codeUsed++;
}

/* Frees all memory allocations in code image. */
void free_code_image(head* arr) {
    free(arr->code_image);
    arr->code_image = NULL;
    arr->codeUsed = arr->codeSize = 0;
}

/* Frees all memory allocations of main DS head. */
void free_head(head* arr) {
    free_symbol_table(arr);
    free_data_image(arr);
    free_code_image(arr);
}

/* Receives symbol name and returns its attribute (R,E) assuming BASE/HIST. If doesn't exist, returns -1. */
int get_attribute(head* headPtr, char* symbol) {
    int i, length;

    length = headPtr->tableUsed;

    for (i = 0; i < length; i++) {
        if (!strcmp(headPtr->table[i].symbol_name, symbol)) {
            if (headPtr->table[i].isExternal)
                return E;
            else
                return R;
        }
    }

    return -1;
}
