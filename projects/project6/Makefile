C = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow -pedantic-errors -fstack-protector-all 
LDFLAGS = -lreadline

all: d8sh

d8sh: d8sh.o lexer.o parser.tab.o executor.o
	$(CC) -o d8sh d8sh.o lexer.o parser.tab.o executor.o $(LDFLAGS)

d8sh.o: d8sh.c executor.h lexer.h 
	$(CC) -c d8sh.c 

lexer.o: lexer.c parser.tab.c
	$(CC) -c lexer.c

parser.tab.o: parser.tab.c command.h 
	$(CC) -c parser.tab.c 

executor.o: executor.c command.h executor.h 
	$(CC) -c executor.c

clean: 
	rm -f *.o d8sh a.out
