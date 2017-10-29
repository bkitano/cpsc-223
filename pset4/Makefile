CC = gcc
CFLAGS = -std=c99 -Wall -g3 -pedantic

Cooccur: cooccur.o smap.o
	${CC} ${CCFLAGS} -o $@ $^
	
cooccur.o: cooccur.c smap.h
	${CC} ${CCFLAGS} -c $^

SmapTest: smap.o
	${CC} ${CCFLAGS} -o $@ $^

WordCount: word_count.o smap.o
	${CC} ${CFLAGS} -o $@ $^
	
word_count.o: word_count.c
	${CC} ${CFLAGS} -c $^

Test: test.o smap.o
	${CC} ${CFLAGS} -o $@ $^

smap.o: smap.c smap.h
	${CC} ${CFLAGS} -c $^
	
test.o: test.c
	${CC} ${CFLAGS} -c $^