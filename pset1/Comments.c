#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    
    int c; // the current character being read from stdin
    
    while( (c = getchar()) != EOF) {
        putchar(c);
    }
    
}

