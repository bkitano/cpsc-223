#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "ldigraph.h"

void read_points(FILE *stream, ldigraph * g);
void search(const ldigraph *g, ldig_search *(method)(const ldigraph *, int), int from, int to);
bool isNumber(char * arg);

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

    ldigraph *g = ldigraph_create(n);
    
    // read lines
    read_points(fptr, g);
    
    for(int i = 2; i < argc; i++) {
        
        if(strcmp(argv[i], "-breadth") == 0) {
            int from = atoi(argv[i+1]);
            int k = i+2;
            while(k < argc && isNumber(argv[k])) {
                int to = atoi(argv[k]);
                search(g, ldigraph_bfs, from, to);
                k++;
            }
        } 
        else if(strcmp(argv[i], "-depth") == 0) {
            int from = atoi(argv[i+1]);
            int k = i+2;
            while(k < argc && isNumber(argv[k])) {
                int to = atoi(argv[k]);
                search(g, ldigraph_dfs, from, to);
                k++;
            }        
        } 
        else if(strcmp(argv[i], "-degree") == 0) {
            int from = atoi(argv[i+1]);
            int k = i+2;
            
            while(k < argc && isNumber(argv[k])) {
                int to = atoi(argv[k]);
                search(g, ldigraph_dfs, from, to);
                k++;
            }   
        } 
    
    }
    
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
        
        // add them
        ldigraph_add_edge(g, x, y);
    }
}


void search(const ldigraph *g, ldig_search *(method)(const ldigraph *, int), int from, int to) {
  ldig_search *s = method(g, from);
  
  if (s != NULL) {
    int len;
    int *path = ldig_search_path(s, to, &len);
    
        if (method == ldigraph_bfs) {
            printf("-breadth ");
        } else if (method == ldigraph_dfs) {
            printf("-depth ");
        } else if (method == ldigraph_ofs) {
            printf("-degree ");
        }
    
    if (path != NULL) {
      printf("%d->%d: [", from, to);
      if(len == 0) {
          printf("%d]\n", from);
      } else {
          for (int i = len; i >= 0; i--) {
            if (i < len) {
              printf(", ");
            }
            printf("%d", path[i]);
          }
          
          printf("]\n");
          free(path);
      }
    } else {
      printf("%d->%d: no path\n", from, to);
    }
    
    ldig_search_destroy(s);
  }
}

bool isNumber(char * arg) {
    int i = 0;
    while (arg[i] != '\0') {
        if(isalpha(arg[i])) {
            return false;
        }
        i++;
    }
    return true;
}