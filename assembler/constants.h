#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_LINE_LENGTH 81
#define MAX_LABEL_LENGTH 31
#define MAX_ARRTIBUTE_LENGTH 15
#define INST_AND_DIR_NUM 20

#define MAX_REGISTERS 16
#define MIN_INDEX_REG 10

typedef enum {
    False,
    True
} bool;

typedef enum {
    MOV,
    CMP,
    ADD,
    SUB,
    LEA,
    CLR,
    NOT,
    INC,
    DEC,
    JMP,
    BNE,
    JSR,
    RED,
    PRN,
    RTS,
    STOP,
    DATA,
    STRING,
    ENTRY,
    EXTERNAL
} operation;

typedef enum {
    IMMEDIATE,
    DIRECT,
    INDEX,
    REG_DIRECT
} addr_method;

typedef struct inst_bin {
    unsigned int dest_addr : 2;
    unsigned int dest_reg : 4;
    unsigned int src_addr : 2;
    unsigned int src_reg : 4;
    unsigned int funct : 4;
    unsigned int attribute : 4;
} inst_bin;

typedef struct {
    unsigned int opcode : 16;
    unsigned int attribute : 4;
} opcode_bin;

typedef union binary {
    inst_bin instruction;
    opcode_bin word;
} binary;

enum attributes {
    E = 1,
    R = 2,
    A = 4
};

enum decodeStatus {
    DONE,
    BASE,
    HIST
};

typedef enum opcodes {
    MOV_OP = 0,
    CMP_OP = 1,

    ADD_OP = 2,
    SUB_OP = 2,

    LEA_OP = 4,

    CLR_OP = 5,
    NOT_OP = 5,
    INC_OP = 5,
    DEC_OP = 5,

    JMP_OP = 9,
    BNE_OP = 9,
    JSR_OP = 9,

    RED_OP = 12,
    PRN_OP = 13,

    RTS_OP = 14,
    STOP_OP = 15
} opcode;

typedef enum {
    ADD_FUNCT = 10,
    SUB_FUNCT = 11,

    CLR_FUNCT = 10,
    NOT_FUNCT = 11,
    INC_FUNCT = 12,
    DEC_FUNCT = 13,

    JMP_FUNCT = 10,
    BNE_FUNCT = 11,
    JSR_FUNCT = 12
} funct;

typedef struct {
    opcode op;
    int funct;

    addr_method src_addr;
    int src_reg;

    addr_method dst_addr;
    int dst_reg;

    int src_imm;
    int dst_imm;

    char src_label[MAX_LABEL_LENGTH];
    char dst_label[MAX_LABEL_LENGTH];

} line_info;

#endif