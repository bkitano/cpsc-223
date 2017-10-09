
CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3

Plist_Unit: plist_unit.c point.c plist.c
	${CC} ${CFLAGS} -o $@ $^ -lm
	
Closest_Pair: closest_pair.c point.c plist.c
	${CC} ${CFLAGS} -o $@ $^ -lm
	
Test: test.c point.c plist.c closest_pair.c
	${CC} ${CFLAGS} -o $@ $^ -lm