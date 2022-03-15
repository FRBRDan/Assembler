#include "preprocessor.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "utils.h"

/* Receives a file with ".as" extension and creates a new one with ".am" extension, with the macros replaced.*/
bool macro_remove(char* filename, char* base_filename) {
    int cmd_length, idx, macro_status;
    bool macroFlag, replacedFlag;
    macroArray macro_array;

    char line[MAX_LINE_LENGTH];
    char macro_name[MAX_MACRO_LENGTH], macro_content[MAX_MACRO_INST];
    char* main_str_ptr;
    char* new_filename;

    FILE* src_file_ptr;
    FILE* dst_file_ptr;

    new_filename = str_with_ext(base_filename, ".am");
    src_file_ptr = fopen(filename, "r");
    if (!(dst_file_ptr = fopen(new_filename, "w")))
        return False;

    macro_content[0] = 0;
    macroFlag = replacedFlag = False;

    init_macro_array(&macro_array, 1);

    while (fgets(line, MAX_LINE_LENGTH, src_file_ptr)) {
        if (empty_string(line)) {
            fputs(line, dst_file_ptr);
            continue;
        }
        main_str_ptr = skip_spaces(line); /* Points to first word. */
        cmd_length = word_length(main_str_ptr);

        macro_status = current_macro_status(main_str_ptr, macroFlag, cmd_length);

        if (macro_status == MACRO_START) {
            macroFlag = True;
            main_str_ptr = skip_spaces(main_str_ptr + cmd_length);
            cmd_length = word_length(main_str_ptr);
            strncpy(macro_name, main_str_ptr, cmd_length); /* Saves macro name */
        } else if (macro_status == MACRO_CONTENT) {
            strncat(macro_content, line, count_line_length(line));
        } else if (macro_status == MACRO_END) {
            macroFlag = False;
            insert_macro_array(&macro_array, macro_name, macro_content);
            macro_content[0] = 0;
        } else {
            for (idx = 0; idx < macro_array.used; idx++) {
                if (strncmp(main_str_ptr, macro_array.array[idx].name, cmd_length) == 0 && empty_string(main_str_ptr + cmd_length)) { /* If it's a name of a macro */
                    fputs(macro_array.array[idx].content, dst_file_ptr);
                    replacedFlag = True;
                }
            }
            /* By this point we know it's not a macro name. And not a definition of a new macro. We copy the line. */
            if (!replacedFlag)
                fputs(line, dst_file_ptr);
            replacedFlag = False;
        }
    }
    free_macro_array(&macro_array);
    free(new_filename);
    fclose(src_file_ptr);
    fclose(dst_file_ptr);
    return True;
}

/* Initializes the macro array. */
void init_macro_array(macroArray* arr, int initial_size) {
    arr->array = (macro*)malloc_with_monitor(initial_size * sizeof(macro));
    arr->used = 0;
    arr->size = initial_size;
}

/* Handles insertion into macro array.*/
void insert_macro_array(macroArray* arr, char* name, char* content) {
    if (arr->used == arr->size) {
        arr->size *= 2;
        arr->array = (macro*)realloc_with_monitor(arr->array, arr->size * sizeof(macro));
    }
    arr->array[arr->used].name = (char*)malloc_with_monitor(strlen(name) + 1);
    arr->array[arr->used].content = (char*)malloc_with_monitor(strlen(content) + 1);
    strcpy(arr->array[arr->used].name, name);
    strcpy(arr->array[arr->used].content, content);
    arr->used++;
}

/* Frees all memory allocations in macro array. */
void free_macro_array(macroArray* arr) {
    int idx;
    for (idx = 0; idx < arr->used; idx++) {
        free(arr->array[idx].name);
        free(arr->array[idx].content);
    }
    free(arr->array);
    arr->array = NULL;
    arr->used = arr->size = 0;
}

/* This function receives the string, a flag and the command length, and returns the current "macro status". */
int current_macro_status(char* str, bool macroFlag, int cmdLength) {
    if (strncmp(str, "macro", strlen("macro")) == 0) { /* First word is 'macro'*/
        return MACRO_START;
    } else if (macroFlag && strncmp(str, "endm", cmdLength)) {
        return MACRO_CONTENT;
    } else if (macroFlag && strncmp(str, "endm", cmdLength) == 0) {
        return MACRO_END;
    }
    return MACRO_NONE;
}