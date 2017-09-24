#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


int main(int argc, char **argv) {
    
    // first argument is the size
    // int n = atoi(argv[1]);
    
    int i;
    for (i = 1; i < argc; i++) {
        printf("%s \n", argv[i]);
    }
    
    // the next arguments are the must-haves
    
    
    // the last arguments are the methods
    
    return 0;
}