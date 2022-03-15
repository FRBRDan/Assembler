#ifndef ERRORS_H
#define ERRORS_H
#include "constants.h"
#include "data_structures.h"

#define MAX_ALNUM 128

/* Macros for common errors: */
#define CONSECUTIVE_COMMAS(str, line, original)                                       \
    if (isConsecutiveComma(str)) {                                                    \
        printf("Error on line %d: %sMultiple consecutive commas.\n", line, original); \
        return True;                                                                  \
    }

#define EXTRANEOUS_TEXT(ch, line, original)                                                 \
    if (ch) {                                                                               \
        printf("Error on line %d: %sExtraneous text after end of line.\n", line, original); \
        return True;                                                                        \
    }

#define COMMA_END(str, line, original)                                                      \
    if (str[strlen(str) - 1] == ',') {                                                      \
        printf("Error on line %d: %sExtraneous text after end of line.\n", line, original); \
        return True;                                                                        \
    }

#define ERR_IMMEDIATE(str, line, original)                                                                                               \
    if (errors_immediate(str)) {                                                                                                         \
        printf("Error on line %d: %sIncorrect immediate number. Must be '#' followed by '+', '-' and integers only.\n", line, original); \
        return True;                                                                                                                     \
    }

#define ERR_INDEX(str, line, original)                                                                                                                              \
    if (errors_index(str)) {                                                                                                                                        \
        printf("Error on line %d: %sIndex operand incorrect. Must be a label name followed by register 10-15 surrounded by square brackets [].\n", line, original); \
        return True;                                                                                                                                                \
    }

#define INVALID_OPERANDS(str, op, line, original)                                    \
    if (is_invalid_operand_num(str, op)) {                                           \
        printf("Error on line %d: %sInvalid number of operands.\n", line, original); \
        return True;                                                                 \
    }

#define INVALID_ADDR_METHOD(errors, line, original)                                 \
    if (errors) {                                                                   \
        printf("Error on line %d: %sInvalid addressing method.\n", line, original); \
        return True;                                                                \
    }

bool errors_in_label(head *, char *, char *, int);
bool is_duplicate_label(head *, char *, int);

bool errors_in_data_line(char *, char *, int, operation);
bool errors_zero_operands_inst(char *, char *, int, operation);
bool errors_one_operand_inst(char *, char *, int, line_info *);
bool errors_two_operands_inst(char *, char *, char *, char *, int, line_info *);

bool errors_immediate(char *);
bool errors_index(char *);
bool is_invalid_operand_num(char *, opcode);
bool illegal_two_operands(addr_method, addr_method, opcode);

bool isCorrectNum(char *);
bool isConsecutiveComma(char *);
bool isExistentFile(char *);

#endif