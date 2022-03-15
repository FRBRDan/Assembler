#include "output.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_structures.h"
#include "errors.h"
#include "first_pass.h"
#include "parse.h"
#include "utils.h"

/* Creates 3 output files: Object File, Entry File, External File. */
void create_output(head* headPtr, char* filename) {
    char *objectFile, *entryFile, *externalFile;

    objectFile = str_with_ext(filename, ".ob");
    entryFile = str_with_ext(filename, ".ent");
    externalFile = str_with_ext(filename, ".ext");

    create_object(headPtr, objectFile);

    create_entry(headPtr, entryFile);

    create_external(headPtr, externalFile);

    free(objectFile);
    free(entryFile);
    free(externalFile);
}

/* Creates an object file. */
void create_object(head* headPtr, char* objectFile) {
    FILE* filePointer;
    int codeLines, dataLines, idx;

    FILE_CREATE(filePointer, objectFile)

    codeLines = headPtr->codeUsed;
    dataLines = headPtr->dataUsed;

    fprintf(filePointer, "\t%d %d\n", codeLines, dataLines);

    for (idx = 0; idx < codeLines; idx++) {
        fprintf(filePointer, "%04d  ", headPtr->code_image[idx].line);
        fprint_format(headPtr, filePointer, idx, True);
    }

    for (idx = 0; idx < dataLines; idx++) {
        fprintf(filePointer, "%04d  ", headPtr->data_image[idx].line);
        fprint_format(headPtr, filePointer, idx, False);
    }

    fclose(filePointer);
}

/* Writes into filePointer according to the required format, independent of compiler/machine type. */
void fprint_format(head* headPtr, FILE* filePointer, int idx, bool isCode) {
    char prefix, delim;
    int i;
    int format[FORMAT_LEN];
    image img = (isCode) ? headPtr->code_image[idx] : headPtr->data_image[idx];

    format[0] = img.bin.instruction.attribute;
    format[1] = img.bin.instruction.funct;
    format[2] = img.bin.instruction.src_reg;
    format[3] = (img.bin.instruction.src_addr << 2) | (img.bin.instruction.dest_reg >> 2);
    format[4] = ((img.bin.instruction.dest_reg << 2) | img.bin.instruction.dest_addr) & 0xF;

    prefix = 'A';
    delim = '-';

    for (i = 0; prefix <= 'E'; i++) {
        delim = (prefix == 'E') ? '\n' : delim;
        fprintf(filePointer, "%c%x%c", prefix, format[i], delim);
        prefix++;
    }
}

/* If labels declared as ".entry" exist, creates an entry file. */
void create_entry(head* headPtr, char* entryFile) {
    FILE* filePointer;
    int tableLength, idx;
    bool isEmpty;

    tableLength = headPtr->tableUsed;
    isEmpty = True;

    FILE_CREATE(filePointer, entryFile)

    for (idx = 0; idx < tableLength; idx++) {
        if (headPtr->table[idx].isEntry) {
            isEmpty = False;
            fprintf(filePointer, "%s, %04d, %04d\n", headPtr->table[idx].symbol_name, headPtr->table[idx].base, headPtr->table[idx].offset);
        }
    }
    fclose(filePointer);
    if (isEmpty)
        remove(entryFile);
}

/* If labels declared as ".extern" exist, creates an extern file. */
void create_external(head* headPtr, char* externalFile) {
    FILE* filePointer;
    int codeLength, idx;
    bool isEmpty;

    codeLength = headPtr->codeUsed;
    isEmpty = True;

    FILE_CREATE(filePointer, externalFile)

    for (idx = 0; idx < codeLength; idx++) {
        if (headPtr->code_image[idx].isExtern && headPtr->code_image[idx].toDecode == BASE) {
            isEmpty = False;
            fprintf(filePointer, "%s BASE %04d\n", headPtr->code_image[idx].label, headPtr->code_image[idx].line);
            fprintf(filePointer, "%s OFFSET %04d\n\n", headPtr->code_image[idx].label, headPtr->code_image[idx].line + 1);
        }
    }

    fclose(filePointer);
    if (isEmpty)
        remove(externalFile);
}