#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "ldigraph.h"

void read_points(FILE *stream, ldigraph * g);

int main(int argc, char **argv) {
    
    FILE *fptr;
    char * filename = argv[1];
 
    /*  open the file for reading */
    fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("Cannot open file \n");
        exit(0);
    }
    
    // read n, the number of points
    int n;
    fscanf(fptr, "%d", &n);
    printf("n: %d\n", n);

    ldigraph *g = ldigraph_create(n);
    
    // read lines
    read_points(fptr, g);

    fclose(fptr);
    ldigraph_destroy(g);
    return 0;
}


/**
 * Reads point from the given stream and adds them to the given list.
 *
 * @param stream a pointer to an input stream
 * @param l a pointer to a list
 * @param n a non-negative integer
 */
void read_points(FILE *stream, ldigraph * g) {

    int x;
    int y;
    
    // while there are lines to read
    while( fscanf(stream, "%d %d \n", &x, &y) == 2 ) {
        
        // print them lol
        printf("from: %d, to: %d\n", x, y);
        
        // add them
        // ldigraph_add_edge(g, x, y);
    }
}