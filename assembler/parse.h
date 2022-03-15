#ifndef PARSE_H
#define PARSE_H
#include "constants.h"
#include "data_structures.h"

enum addiotonal_words { ZERO_WORDS,
                        ONE_WORD,
                        TWO_WORDS_IMMEDIATE,
                        TWO_WORDS,
                        THREE_WORDS,
                        FOUR_WORDS };

int parse_data_line(head *, char *, int, operation);
int parse_inst_line(head *, char *, char *, char *, int, operation, bool *, int);

bool is_comment(char *);
int getArrayLength(char *);
char *nextNum(char *);

bool isImmediate(char *, line_info *, bool);
bool isDirect(char *, line_info *, bool);
bool isIndex(char *, line_info *, bool);
bool isRegister(char *, line_info *, bool);

void updateFunctAndOpcode(operation, line_info *);
addr_method operandMethod(char *, line_info *, bool);
int howManyWords(addr_method, addr_method);

#endif