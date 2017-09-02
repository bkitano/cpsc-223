#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    FILE *f; // file type decl.
    int x; // the current read number
    int sum; // the running total
    
    // makes sure you name the file
    if(argc < 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(1);
    }
    
    // read the file
    f = fopen(argv[1], "r"); // read priviliges
    
    // if there's an error reading the file
    if(f == 0) {
        // errors
        exit(2);
    }
    
    // all is good!
    sum = 0;
    
    // while there are more numbers
    while(fscanf(f, "%d", &x) == 1 ){
        sum += x; // sum
    }

    printf("%d\n", sum);
    
    // close the read document
    fclose(f);
    
    return 0;
}