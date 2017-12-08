CFLAGS = -std=c99 -Wall -g3 -pedantic
CC = gcc

test:
	make t1
	valgrind --tool=memcheck --leak-check=full -q ./t1

t1: chomp.o
	${CC} ${CFLAGS} -o $@ $^

chomp.o: chomp.c chomp.h
	${CC} ${CFLAGS} -c $^