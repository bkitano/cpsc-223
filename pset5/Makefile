CC = gcc
CFLAGS = -std=c99 -Wall -pedantic -g3

tvgrind:
	make Test
	valgrind --tool=memcheck --leak-check=full -q ./Test a 1 a 2 a 3 a 4 a 5

dvgrind:
	make debug
	valgrind --tool=memcheck --leak-check=yes -q ./debug
	
grundy: grundy.o isset.o
	${CC} ${CFLAGS} -o $@ $^
	
grundy.o: grundy.c isset.h
	${CC} ${CFLAGS} -c $^
	
debug: isset.o
	${CC} ${CFLAGS} -o $@ $^

Test: isset_test.o isset.o
	${CC} ${CFLAGS} -o $@ $^

isset_test.o: isset_test.c isset.h
	${CC} ${CFLAGS} -c $^
	
isset.o: isset.c isset.h
	${CC} ${CFLAGS} -c $^
	