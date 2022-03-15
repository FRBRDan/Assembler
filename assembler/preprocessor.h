#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "constants.h"
#define MAX_MACRO_LENGTH 20
#define MAX_MACRO_INST 100 /* Maximum length of content assumed. */

typedef struct {
    char* content;
    char* name;
} macro;

typedef struct {
    macro* array;
    int used;
    int size;
} macroArray;

enum { MACRO_START,
       MACRO_CONTENT,
       MACRO_END,
       MACRO_NONE };

bool macro_remove(char*, char*);

void init_macro_array(macroArray*, int);
void insert_macro_array(macroArray*, char*, char*);
void free_macro_array(macroArray*);

int current_macro_status(char*, bool, int);

#endif
