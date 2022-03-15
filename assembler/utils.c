#include "utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Joins two strings together. This allocates memory, and needs to be freed after use. */
char* str_with_ext(char* str1, char* ext) {
    char* united_string = (char*)malloc_with_monitor(strlen(str1) + strlen(ext) + 1);
    strcpy(united_string, str1);
    strcat(united_string, ext);
    return united_string;
}

/* Malloc with monitor - if memory allocation is failed, exits the program. */
void* malloc_with_monitor(long size) {
    void* p = malloc(size);
    if (!p) {
        printf("Error: Failed to allocate memory.\n");
        exit(1);
    }
    return p;
}

/* Realloc with monitor - if memory reallocation is failed, exits the program. */
void* realloc_with_monitor(void* start, long size) {
    void* p = realloc(start, size);
    if (!p) {
        printf("Error: Failed to reallocate memory.\n");
        exit(1);
    }
    return p;
}

/* Returns pointer right after spaces and tabs. */
char* skip_spaces(char* str) {
    int i = 0;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    return str + i;
}

/* Counts length of word (until space-character or NULL character is reached). */
int word_length(char* str) {
    int i = 0;
    for (; !isspace(str[i]) && str[i]; i++)
        ;
    return i;
}

/* Counts length of line (until \n or NULL character is reached). */
int count_line_length(char* p) {
    int i = 0;
    while (p[i] != '\n' && p[i])
        i++;
    return i + 1;
}

/* Returns true if the string received is empty. */
bool empty_string(char* str) {
    char* p = skip_spaces(str);
    if (*p == '\n' || !*p)
        return True;
    return False;
}

/* Returns a pointer right after the current word. */
char* skip_word(char* str) {
    int idx = 0;
    while (!isspace(str[idx]) && str[idx]) {
        idx++;
    }
    return str + idx;
}

/* Deletes the spaces, tabs and newline in a given string. */
void delete_spaces(char* p) {
    int i = 0, count = 0;
    bool str_flag = False;
    while (p[i]) {
        if (p[i] == '\"' && str_flag)
            str_flag = False;
        else if (p[i] == '\"' && !str_flag)
            str_flag = True;

        if (!isspace(p[i]) || (p[i] == ' ' && str_flag))
            p[count++] = p[i];
        i++;
    }
    p[count] = '\0'; /* End the string. */
}

/* Returns hist, given a number. */
int getHist(int value) {
    return value % 16;
}

/* Returns base, given a number. */
int getBase(int value) {
    return value - getHist(value);
}