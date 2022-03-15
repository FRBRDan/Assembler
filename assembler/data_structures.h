#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H
#include "constants.h"

typedef struct {
    char symbol_name[MAX_LABEL_LENGTH];
    char atrributes[MAX_ARRTIBUTE_LENGTH]; /* Maybe not needed considering the bool */
    int value;
    int base;
    int offset;

    bool isExternal;
    bool isCode;
    bool isData;
    bool isEntry;
} symbol;

typedef struct {
    int line;
    char label[MAX_LABEL_LENGTH];
    int toDecode;
    bool isExtern;
    binary bin;
} image;

typedef struct {
    symbol* table;
    image* data_image;
    image* code_image;

    int tableUsed;
    int tableSize;

    int dataUsed;
    int dataSize;

    int codeUsed;
    int codeSize;
} head;

void head_init(head*, int, int, int);
void image_init(image*);
void symbol_init(symbol*);

/* Symbol table functions */
void insert_symbol(head*, char*, int, int, int, operation);
void insert_data_symbol(head*, char*, int, operation);
void insert_extern(head*, char*, operation);
void insert_code_symbol(head*, char*, int, operation);
void free_symbol_table(head*);

/* Data image functions */
void insert_data_img(head*, unsigned int, int);
void update_data_count(head*, int);
void free_data_image(head*);

/* Code image functions */
void insert_code_opcode(head*, unsigned int, unsigned int, int);
void insert_code_instruction(head*, line_info*, int);
void insert_code_immediate(head*, int, int, int, char*, int);
void free_code_image(head*);

void free_head(head*);

int get_attribute(head*, char*);

#endif