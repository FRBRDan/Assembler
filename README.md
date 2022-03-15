# C Project: 20-bit Assembler

## About

This two-pass assembler was written for an imaginary computer as a final project of the course *[20465 - Systems Programming in C](https://openu.ac.il/courses/20465.htm)*.

## Usage

1. Run `make`
2. Run `./assembler fileName1 fileName2 ...`

The assembler will output *fileName.am*, *fileName.ent*, *fileName.ext* and *fileName.ob*.

## Structure

- `assembler.c` - main function definition, argv & argc processing, single file processing.
- `preprocessor.c` - deals with macro-spreading and creates a *.am* file.
- `first_pass.c` - implements the first pass algorithm.
- `parse.c` - parses each line, as part of `first_pass.c`.
- `second_pass.c` - implements the second pass algorithm.
- `data_structures.c` - implementation of dynamic array for symbol table, code and data images.
- `errors.c` - contains functions to thoroughly check for errors in the *.am* file.
- `output.c` - creates the aforementioned output files.
- `utils.c` - general all-purpose functions.
- `constants.h` - implementation of constant values.

## Computer and Language Structure

### Computer Structure
The imaginary computer consists of CPU, Registers and RAM (some of the RAM is utilized as stack).

The CPU has 16 registers (r0-r15). each register size is 20 bits. LSB is 0 and MSB is 19.

The CPU has a register named PSW that contains flags regarding the PC status in each moment.

Memory size is 8192 and each memory cell size is 20 bits.

The computer only works with integers.

### Word and Sentence Structure

Each computer instruction adds between 1 to 6 words which are encoded in the following manner:

| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
|  destination address | destination address  |  destination | destination  | destination | destination | source address | source address | source | source | source | source | funct | funct | funct | funct | attribute | attribute | attribute | attribute |

This model consists of 16 operations, each of them has a different funct and opcode (*mov, cmp, add, sub, lea, clr, not, inc, dec, jmp, bne, jsr, red, prn, rts and stop*).

There are 4 kinds of sentences the assembler recognizes:

1. Empty Sentence - A line that only contains whitespaces.

2. Comment Sentence - A line that begins with `;`.

3. Instruction Sentence - Variables assignment and declaration.

4. Command Sentence - Creates an action for the machine to execute upon running the program.

Maximum line length is 80. 

Usage of labels is optional. A label is any word (reserved words not allowed) which is declared at the beginning of the sentence and ends with `:`. For example `myLabel:`. It cannot start with a digit, and has to be at most 31 characters long.

#### Instruction Sentence

Instruction Sentence may or may not start with a label. Valid instructions are: 

1. **.data** - declaration of integers. For example: `.data 58, +3, -92`.

2. **.string** - declaration of a string wrapped within *" "*. For example: `.string "This is a string."`.

3. **.extern** - reference to an external label, declared in another file. For example `.extern externalLabel`.

4. **.entry** - reference to an internal label, that must be declared in the program. For example `.entry entryLabel`.

#### Command Sentence

Command Sentence may or may not start with a label. Valid commands are: 

**mov** - copies origin operand to destination

**cmp** - performs comparison between 2 operands

**add** - destination operand receives the addition result of origin and destination

**lea** - load effective address

**clr** - clears operand

**not** - logical not, reverses all bits in operand

**inc** - increments operand's content by 1

**dec** - decrements operand's content by 1

**jmp** - jumps to instruction in destination operand

**bne** - branch if not equal to zero

**jsr** - calls a subroutine

**red** - reads a char from stdin

**prn** - prints char into stdout

**rts** - returns from subroutine

**stop** - stops the program

## The Algorithm

The assembler receives an input in assembly language. It goes over the input twice. The reason for that is because there are references to labels and instructions that have unknown addresses during the first pass.

The assembler implements a dynamic array for spreading the macros. It also implements a Data Structure that joins together 3 dynamic arrays. One is used for the Symbol Table - which stores the addresses of the labels declared. And the other two are used for the Code Image, and Data Image.

The Code Image stores the command sentences in machine code, and the Data Image stores the instruction sentences in machine code.

The machine code is essentially built during first pass, though operands of type "DIRECT" that refer to labels, are updated during the second pass. Through it all, errors are thoroughly checked. If any errors are found during first pass, or second pass, the program prints the errors and continues to the next file. 

