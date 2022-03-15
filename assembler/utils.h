#ifndef UTILS_H
#define UTILS_H
#include "constants.h"

char* str_with_ext(char*, char*);

void* malloc_with_monitor(long);
void* realloc_with_monitor(void*, long);

char* skip_spaces(char*);
int word_length(char*);
int count_line_length(char*);
bool empty_string(char*);
char* skip_word(char*);
void delete_spaces(char*);

int getHist(int);
int getBase(int);
#endif
