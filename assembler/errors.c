#include "errors.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

/* Checks if the label is correct. Assumes line has a label. */
bool errors_in_label(head* headPtr, char* original_line, char* line, int lineNumber) {
    char* ptr;
    bool errors;
    int i, length, reg;
    char* opNames[INST_AND_DIR_NUM] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop", ".data", ".string", ".entry", ".extern"};

    errors = False;
    i = 0;
    ptr = line;

    while (ptr[i] != ':')
        i++;

    length = i;

    if (length > 31 || isdigit(ptr[0]))
        errors = True;

    for (i = 0; i < length; i++) {
        if (!isalnum(ptr[i]))
            errors = True;
    }

    for (i = 0; i < INST_AND_DIR_NUM; i++) {
        if (strlen(opNames[i]) == length && !strncmp(line, opNames[i], length))
            errors = True;
    }

    reg = atoi(ptr + 1);

    if (ptr[0] == 'r' && ((reg >= 0 && reg < MIN_INDEX_REG && length == strlen("r0")) || (reg >= MIN_INDEX_REG && reg < MAX_REGISTERS && length == strlen("r10"))))
        errors = True;

    if (is_duplicate_label(headPtr, line, length)) {
        printf("Error on line %d: %sDuplicate label name. The name has already been used.\n", lineNumber, original_line);
    }

    if (errors) {
        printf("Error on line %d: %sLabel has to be at most 31 characters long, first character has to be a letter, cannot be a saved word and all characters have to be alphaneumeric.\n", lineNumber, original_line);
        return True;
    }
    return False;
}

/* Returns True if the label already exists in the Symbol Table. */
bool is_duplicate_label(head* headPtr, char* line, int length) {
    int i;
    for (i = 0; i < headPtr->tableUsed; i++)
        if (strlen(headPtr->table[i].symbol_name) == length && !strncmp(line, headPtr->table[i].symbol_name, length))
            return True;

    return False;
}

/* Returns True if there are errors in a data line, where line points to first element after ".string"/".data". */
bool errors_in_data_line(char* original_line, char* line, int lineNumber, operation op) {
    char *ptr, *token;
    char line_copy[MAX_LINE_LENGTH];
    bool errors;
    int i = 0;

    ptr = line;

    errors = False;

    if (op == STRING) {
        if (ptr[i++] != '\"')
            errors = True;

        while (ptr[i] != '\"' && ptr[i]) {
            if (!(ptr[i] > 0 && ptr[i] < MAX_ALNUM))
                errors = True;
            i++;
        }
        if (!ptr[i])
            errors = True;

        if (!errors) /* Ensures we are in range of array. */
            EXTRANEOUS_TEXT(ptr[i + 1], lineNumber, original_line)

        if (errors) {
            printf("Error on line %d: %sString has to contain ASCII characters surrounded by parentheses (\").\n", lineNumber, original_line);
            return True;
        }
    } else if (op == DATA) {
        if (!ptr[i])
            errors = True;
        strcpy(line_copy, line);

        CONSECUTIVE_COMMAS(line, lineNumber, original_line)

        COMMA_END(line, lineNumber, original_line)

        token = strtok(line_copy, ",");

        while (token != NULL) {
            errors = (isCorrectNum(token)) ? errors : True;
            token = strtok(NULL, ",");
        }
        if (errors) {
            printf("Error on line %d: %sArray of type \".data\" has to contain integer numbers, separated by commas.\n", lineNumber, original_line);
            return True;
        }
    }
    return False;
}

/* Returns True if errors found in line with zero operands. */
bool errors_zero_operands_inst(char* original_line, char* line, int lineNumber, operation op) {
    if (op == -1) {
        printf("Error on line %d: %sOperation not recognized.\n", lineNumber, original_line);
        return True;
    }

    if (op == RTS || op == STOP)
        EXTRANEOUS_TEXT(line[0], lineNumber, original_line)
    return False;
}

/* Returns True if errors found in line with one operand. */
bool errors_one_operand_inst(char* original_line, char* line, int lineNumber, line_info* instruction) {
    /* Only destination matters. REG_DIRECT that is incorrect considered as DIRECT.*/
    bool errors = False;

    INVALID_OPERANDS(line, instruction->op, lineNumber, original_line)
    COMMA_END(line, lineNumber, original_line)

    if (instruction->dst_addr == IMMEDIATE) {
        ERR_IMMEDIATE(line, lineNumber, original_line)
    } else if (instruction->dst_addr == INDEX) {
        ERR_INDEX(line, lineNumber, original_line)
    }

    if ((instruction->dst_addr == IMMEDIATE && instruction->op != PRN_OP) || (instruction->dst_addr == REG_DIRECT && instruction->op == JMP_OP))
        errors = True;

    INVALID_ADDR_METHOD(errors, lineNumber, original_line)

    return False;
}

/* Returns True if errors found in line with two operands. */
bool errors_two_operands_inst(char* original_line, char* line, char* first_word, char* second_word, int lineNumber, line_info* instruction) {
    bool errors;

    errors = False;

    INVALID_OPERANDS(line, instruction->op, lineNumber, original_line)
    COMMA_END(line, lineNumber, original_line)
    CONSECUTIVE_COMMAS(line, lineNumber, original_line)

    /* REG_DIRECT has already been checked. DIRECT will be checked later. */
    if (instruction->src_addr == IMMEDIATE) {
        ERR_IMMEDIATE(first_word, lineNumber, original_line)
    } else if (instruction->src_addr == INDEX) {
        ERR_INDEX(first_word, lineNumber, original_line)
    }

    if (instruction->dst_addr == IMMEDIATE) {
        ERR_IMMEDIATE(second_word, lineNumber, original_line)
    } else if (instruction->dst_addr == INDEX) {
        ERR_INDEX(second_word, lineNumber, original_line)
    }

    if (illegal_two_operands(instruction->src_addr, instruction->dst_addr, instruction->op))
        errors = True;

    INVALID_ADDR_METHOD(errors, lineNumber, original_line)

    return False;
}

/* Assumes first character is '#'. Returns True if the string is not a legal integer number. */
bool errors_immediate(char* str) {
    int i = 1;
    if (!isdigit(str[i]) && str[i] != '+' && str[i] != '-')
        return True;
    while (str[++i])
        if (!isdigit(str[i]))
            return True;
    return False;
}

/* Assumes there is '[' sign in the string. Returns true if there are errors as an index. */
bool errors_index(char* str) {
    int i, reg;

    i = 0;

    while (str[i++] != '[')
        ;
    if (str[i++] != 'r')
        return True;

    reg = atoi(str + i);

    if (!(reg >= MIN_INDEX_REG && reg < MAX_REGISTERS) || str[i + 2] != ']' || str[i + 3])
        return True;

    return False;
}

/* Returns true if there's an invalid amount of operands in the line. Does not deal with RTS/STOP. */
bool is_invalid_operand_num(char* str, opcode code) {
    char line_copy[MAX_LINE_LENGTH];
    char* token;

    strcpy(line_copy, str);
    token = strtok(line_copy, ",");

    if (code >= CLR_OP && code <= PRN_OP) { /* One operand */
        if ((token = strtok(NULL, ",")) != NULL)
            return True;
    } else if (code >= MOV_OP && code <= LEA_OP)
        if ((token = strtok(NULL, ",")) == NULL)
            return True;

    return False;
}

/* Returns True if the combination of operands received is illegal. */
bool illegal_two_operands(addr_method src, addr_method dst, opcode code) {
    if (((src == IMMEDIATE || src == REG_DIRECT) && code == LEA_OP) || (dst == IMMEDIATE && code != CMP_OP))
        return True;
    return False;
}
/* Checks if a string is a correct number - starts with '+' or '-' followed by digits or all digits. */
bool isCorrectNum(char* str) {
    int i, length;

    i = 0;

    if (!isdigit(str[i]) && str[i] != '+' && str[i] != '-')
        return False;

    length = strlen(str);

    for (i = 1; i < length; i++) {
        if (!isdigit(str[i]))
            return False;
    }
    return True;
}

/* Returns True if there are two (or more) consecutive commas in the string provided. */
bool isConsecutiveComma(char* str) {
    int i = 0;
    for (i = 0; i < strlen(str); i++) {
        if (str[i] == ',' && str[i + 1] == ',')
            return True;
    }
    return False;
}

/* Returns True if the filename provided exists, otherwise returns False. */
bool isExistentFile(char* filename) {
    FILE* filePointer;

    filePointer = fopen(filename, "r");

    if (!filePointer) {
        printf("Error: File named \"%s\" not found. Continuing to the next one.\n\n", filename);
        return False;
    }
    fclose(filePointer);
    return True;
}
