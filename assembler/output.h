#ifndef OUTPUT_H
#define OUTPUT_H
#include <stdio.h>

#include "constants.h"
#include "data_structures.h"

#define FORMAT_LEN 5

#define FILE_CREATE(filePointer, name)            \
    if (!(filePointer = fopen(name, "w"))) {      \
        printf("Cannot create file %s.\n", name); \
        return;                                   \
    }

void create_output(head*, char*);
void create_object(head*, char*);

void fprint_format(head*, FILE*, int, bool);

void create_entry(head*, char*);
void create_external(head*, char*);

#endif