#include <stdio.h>
#include <ctype.h>

int peekchar(void); // just for safety, decalre it at the top for wide scope
int readNumber(void);

/*returns the next character from stdin
without consuming it */

int peekchar(void) {
    int c; // the declaration
    
    c = getchar(); // read in the first char
    if(c != EOF) { // if it's not the end of file
        ungetc(c, stdin); // put it back lol
    }
    
    return c;
}

/* read an integer written in decimal
notation until the first non-digit and
returns it. */

int readNumber(void) {
    int accumulator; // the number so far
    int c; // the next character
    
    accumulator = 0; // set default return to 0
    
    while( (c=peekchar()) != EOF && isdigit(c)) {
        c = getchar();
        accumulator *= 10;
        accumulator += (c-'0');
    }
    
    return accumulator;
}

int main(int argc, char **argv) {
    int x;
    x = readNumber();
    printf("%d\n",x);
    
    return 0;
}
