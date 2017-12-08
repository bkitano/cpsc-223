CFLAGS = -std=c99 -Wall -g3 -pedantic
CC = gcc

test2: 
	make Chomp
	valgrind --leak-check=yes --tool=memcheck -q ./Chomp 33322

Chomp: chomp_main.o chomp.o smap.o string_helpers.o
	${CC} ${CFLAGS} -o $@ $^ 

chomp_main.o: chomp_main.c chomp.h string_helpers.h smap.h
	${CC} ${CFLAGS} -c $^
	
smap.o: smap.c smap.h
	${CC} ${CFLAGS} -c $^
	
string_helpers.o: string_helpers.c string_helpers.h
	${CC} ${CFLAGS} -c $^

test1:
	make t1
	valgrind --tool=memcheck --leak-check=full -q ./t1

t1: chomp.o
	${CC} ${CFLAGS} -o $@ $^

chomp.o: chomp.c chomp.h
	${CC} ${CFLAGS} -c $^