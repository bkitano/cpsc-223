CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3

stest:
	make search
	valgrind --leak-check=yes --tool=memcheck -q ./search

search: search.o ldigraph.o
	${CC} ${CFLAGS} -o $@ $^
	
search.o: search.c
	${CC} ${CFLAGS} -c $^

test: ldigraph.o
	${CC} ${CFLAGS} -o $@ $^
	
ldigraph.o: ldigraph.c ldigraph.h
	${CC} ${CFLAGS} -c $^
	
