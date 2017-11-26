#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "ldigraph.h"

void read_points(FILE *stream, int n, ldigraph * g);

int main(int argc, char **argv) {
    
    FILE *fptr;
    char * filename = argv[1];
    char ch;
 
    /*  open the file for reading */
    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
    
    // read n, the number of points
    int n;
    fscanf(fptr, "%d", &n);
    printf("%d\n", n);

    // ldigraph *g = ldigraph_create(n);
    
    // // read into one list
    // read_points(stdin, n, g);

    fclose(fptr);
    return 1;
}


/**
 * Reads point from the given stream and adds them to the given list.
 *
 * @param stream a pointer to an input stream
 * @param l a pointer to a list
 * @param n a non-negative integer
 */
void read_points(FILE *stream, int n, ldigraph * g) {
  for(int i = 0; i < n; i++) {
    
    int x;
    int y;
    
    fscanf(stream, "%d %d \n", &x, &y);
    
    printf("from: %d, to: %d\n", x,y);
    
    // ldigraph_add_edge(g, x, y);
    
  }
}