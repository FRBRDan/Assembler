MAIN_FLAG = gcc -g -Wall -ansi -pedantic

OBJ_FLAG = gcc -c -Wall -ansi -pedantic

OBJECTS = assembler.o utils.o preprocessor.o data_structures.o first_pass.o second_pass.o parse.o errors.o output.o

HEADERS = assembler.h utils.h preprocessor.h constants.h data_structures.h first_pass.h second_pass.h parse.h errors.h output.h

assembler: $(OBJECTS)
	$(MAIN_FLAG) $(OBJECTS) -o assembler

assembler.o: assembler.c $(HEADERS)
	$(OBJ_FLAG) assembler.c -o assembler.o

preprocessor.o: preprocessor.c $(HEADERS)
	$(OBJ_FLAG) preprocessor.c -o preprocessor.o

data_structures.o: data_structures.c $(HEADERS)
	$(OBJ_FLAG) data_structures.c -o data_structures.o

first_pass.o: first_pass.c $(HEADERS)
	$(OBJ_FLAG) first_pass.c -o first_pass.o

second_pass.o: second_pass.c $(HEADERS)
	$(OBJ_FLAG) second_pass.c -o second_pass.o

parse.o: parse.c $(HEADERS)
	$(OBJ_FLAG) parse.c -o parse.o

errors.o: errors.c $(HEADERS)
	$(OBJ_FLAG) errors.c -o errors.o

utils.o: utils.c $(HEADERS)
	$(OBJ_FLAG) utils.c -o utils.o

output.o: output.c $(HEADERS)
	$(OBJ_FLAG) output.c -o output.o

clean:
	rm *.o
	
