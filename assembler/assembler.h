#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define ERRORS_FIRST_PASS(errors)                    \
    if (errors) {                                    \
        printf("Errors found during first pass!\n"); \
        free(updated_filename);                      \
        free_head(&headPointer);                     \
        return;                                      \
    }

#define EXISTENT_FILE(filename)      \
    if (!isExistentFile(filename)) { \
        free(filename);              \
        return;                      \
    }

#define PREASSEMBLER(updated_filename, filename)     \
    if (!macro_remove(updated_filename, filename)) { \
        free(updated_filename);                      \
        return;                                      \
    }

#define ERROR_FREE_AND_REMOVE(filename, headPtr) \
    {                                            \
        remove(filename);                        \
        free(filename);                          \
        free_head(&headPtr);                     \
    }

void process_file(char*);

#endif