#include "second_pass.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_structures.h"
#include "errors.h"
#include "first_pass.h"
#include "parse.h"
#include "utils.h"

/* The function is in charge of the second pass. It returns False if no errors were found, and updates the headPointer accordingly. */
bool process_second_pass(head* headPtr, char* filename) {
    char line[MAX_LINE_LENGTH], original_line[MAX_LINE_LENGTH];
    int line_num, shift;
    char* wordPointer;
    FILE* filePointer;
    bool errorsFound;
    operation op;

    line_num = 0;
    errorsFound = False;
    filePointer = fopen(filename, "r");

    while (fgets(line, MAX_LINE_LENGTH, filePointer)) {
        line_num++;
        if (empty_string(line) || is_comment(line))
            continue;

        strcpy(original_line, line);

        wordPointer = skip_spaces(line);

        if ((shift = label_check(wordPointer, NULL)) != -1) {
            wordPointer += shift;
            wordPointer = skip_spaces(wordPointer);
        }

        op = firstWord(wordPointer);
        if (op != ENTRY)
            continue;

        wordPointer = skip_word(wordPointer);
        delete_spaces(wordPointer);

        errorsFound = (insert_entry(headPtr, wordPointer, original_line, line_num)) ? errorsFound : True;
    }
    fclose(filePointer);

    errorsFound = (update_code_symbols(headPtr)) ? True : errorsFound;

    return errorsFound;
}

/* The function searches the symbol table for the label, and adds the entry attribute to it. Returns False if not found in table or if duplicate external label is found. */
bool insert_entry(head* headPtr, char* label, char* line, int lineNumber) {
    int idx, arrLength;
    bool insertedFlag;

    insertedFlag = False;
    arrLength = headPtr->tableUsed;

    for (idx = 0; idx < arrLength; idx++) {
        if (!strcmp(headPtr->table[idx].symbol_name, label)) {
            if (headPtr->table[idx].isExternal) {
                printf("Error on line %d: %sLabel declared as ENTRY and EXTERN simultaneously.\n", lineNumber, line);
                return False;
            }
            headPtr->table[idx].isEntry = True;
            insertedFlag = True;
        }
    }
    if (insertedFlag)
        return True;

    printf("Error on line %d: %sLabel stated after .entry does not exist.\n", lineNumber, line);
    return False;
}

/* Updates the code image with BASE/HIST. It returns True if errors were found, and False upon success. */
bool update_code_symbols(head* headPtr) {
    int i, length, value, attribute;
    bool errors;

    value = attribute = 0;
    errors = False;
    length = headPtr->codeUsed;

    for (i = 0; i < length; i++) {
        attribute = get_attribute(headPtr, headPtr->code_image[i].label);
        if (attribute == E)
            headPtr->code_image[i].isExtern = True;

        if (headPtr->code_image[i].toDecode == BASE) {
            if ((value = get_base_or_hist(headPtr, headPtr->code_image[i].label, BASE)) == -1) {
                printf("Error: Label \"%s\" was not configured - does not exist in symbol table.\n", headPtr->code_image[i].label);
                errors = True;
            }
            headPtr->code_image[i].bin.word.attribute = attribute;
            headPtr->code_image[i].bin.word.opcode = value;
        } else if (headPtr->code_image[i].toDecode == HIST && !errors) {
            value = get_base_or_hist(headPtr, headPtr->code_image[i].label, HIST);
            headPtr->code_image[i].bin.word.attribute = attribute;
            headPtr->code_image[i].bin.word.opcode = value;
        }
    }

    return errors;
}

/* Returns BASE or HIST of symbol. If symbol doesn't exist in symbol table, returns -1. */
int get_base_or_hist(head* headPtr, char* symbol, int decode) {
    int i, length, value;

    length = headPtr->tableUsed;

    for (i = 0; i < length; i++) {
        if (!strcmp(headPtr->table[i].symbol_name, symbol)) {
            value = (decode == BASE) ? headPtr->table[i].base : headPtr->table[i].offset;
            return value;
        }
    }

    return -1; /* Not found. */
}
