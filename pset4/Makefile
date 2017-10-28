CC = gcc
CFLAGS = -std=c99 -Wall -g3 -pedantic

Test: test.o smap.o
	${CC} ${CFLAGS} -o $@ $^

smap.o: smap.c smap.h
	${CC} ${CFLAGS} -c $^
	
test.o: test.c
	${CC} ${CFLAGS} -c $^