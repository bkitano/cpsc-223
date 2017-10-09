
CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3

Plist_Unit: plist_unit.o point.o plist.o
	${CC} ${CFLAGS} -o $@ $^ -lm
	
Closest_Pair: closest_pair.o point.o plist.o
	${CC} ${CFLAGS} -o $@ $^ -lm
	
Test: test.o point.o plist.o closest_pair.o
	${CC} ${CFLAGS} -o $@ $^ -lm