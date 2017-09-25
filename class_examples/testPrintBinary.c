#include <stdio.h>
#include <stdlib.h>

/* print out all bits of n */

void print_binary(unsigned int n) {
    unsigned int mask = 0;
    
    mask = ~mask ^ (~mask >> 1);
    
    /*this creates 100000000--- regardless of size*/
    for(; mask != 0; mask >>= 1) {
        putchar((n & mask) ? '1' : '0'); // not entirely sure what this is
    }
}

int main(int argc, char **argv) {
    
    if(argc != 2) {
        fprintf(stderr, "Usage: %s n\n", argv[0]);
        return 1;
    }
    
    print_binary(atoi(argv[1]));
    putchar('\n');
    
    return 0;
}
