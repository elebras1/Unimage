CC = gcc
CFLAGS = -W -Wall -Werror

prog : stack2.o testStack2.o
	$(CC) stack2.o testStack2.o -o prog

stack2.o : stack2.o stack2.h

testStack2.o : testStack2.c
	$(CC) -c testStack2.c

clear: 
	rm -f testStack.o prog
