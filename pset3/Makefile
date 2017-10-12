CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3

ClosestPair: closest_pair.o point.o plist.o
	${CC} ${CFLAGS} -o $@ $^ -lm

closest_pair.o: closest_pair.c
	${CC} ${CFLAGS} -c closest_pair.c -lm
	
PlistUnit: plist_unit.o point.o plist.o
	${CC} ${CFLAGS} -o $@ $^ -lm

plist_unit.o: plist_unit.c plist.h
	${CC} ${CFLAGS} -c plist_unit.c -lm

point.o: point.c point.h
	${CC} ${CFLAGS} -c point.c -lm

plist.o: plist.c plist.h
	${CC} ${CFLAGS} -c plist.c -lm