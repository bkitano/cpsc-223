#include <stdio.h>
#include <stdlib.h>

#define SINGLE_QUOTE_ASCII 39
#define DOUBLE_QUOTE_ASCII 34
#define NEW_LINE_ASCII 10
#define BACKSLASH_ASCII 92
#define FORWARDSLASH_ASCII 47

typedef enum {
    SINGLE_LINE, SINGLE_LINE_JAVADOC, SINGLE_LINE_LEADING_CHARACTERS,
    EXTENDED_COMMENT, EXTENDED_JAVADOC, EXTENDED_LEADING_CHARACTERS,
    STRING_LITERAL_SUPPRESS, REGULAR_CODE
} state;

int peekchar(void);

int main() {
    
    int c; // the character being read
    state curr = REGULAR_CODE; // initialize the state at REGULAR_CODE
        
    while ( (c = getchar()) != EOF) {
    
        int d = peekchar(); // the next character
    
        switch (curr) {
            
            case REGULAR_CODE:
                switch(c) {
                    case FORWARDSLASH_ASCII:
                        switch(d) {
                            case '/':
                                curr = SINGLE_LINE_LEADING_CHARACTERS;
                                break;
                            case '*':
                                curr = EXTENDED_LEADING_CHARACTERS;
                                break;
                        }
                        break;
                    case DOUBLE_QUOTE_ASCII:
                        curr = STRING_LITERAL_SUPPRESS;
                        break;
                }
                break;
        
            case STRING_LITERAL_SUPPRESS:
                if(c == DOUBLE_QUOTE_ASCII && d != BACKSLASH_ASCII) {
                    curr = REGULAR_CODE;
                }
                break;
        
            case EXTENDED_LEADING_CHARACTERS:
                switch(c) {
                    case '*':
                        curr = EXTENDED_LEADING_CHARACTERS;
                        break;
                    case ' ':
                        curr = EXTENDED_LEADING_CHARACTERS;
                        break;
                    case '@':
                        curr = EXTENDED_JAVADOC;
                        break;
                    default:
                        putchar(c);
                        curr = EXTENDED_COMMENT;
                        break;
                }
                break;
        
            case SINGLE_LINE_LEADING_CHARACTERS:
                switch(c) {
                    case '*':
                        curr = SINGLE_LINE_LEADING_CHARACTERS;
                        break;
                    case ' ':
                        curr = SINGLE_LINE_LEADING_CHARACTERS;
                        break;
                    case '@':
                        curr = SINGLE_LINE_JAVADOC;
                        break;
                    default:
                        putchar(c);
                        curr = SINGLE_LINE;
                        break;
                }
                break;
                
            case EXTENDED_COMMENT:
                switch(c) {
                    case ' ':
                        if(d == '@') {
                            curr = EXTENDED_JAVADOC;
                        } else {
                            putchar(c);
                        }
                        break;
                    case '*':
                        if(d == '/') {
                            putchar(NEW_LINE_ASCII);
                            curr = REGULAR_CODE;
                        } else {
                            putchar(c);
                        }
                        break;
                    case BACKSLASH_ASCII:
                        getchar();
                        break;
                    default:
                        putchar(c);
                        break;
                }
                break;
        
            case EXTENDED_JAVADOC:
                switch(c) {
                    case ' ':
                        curr = EXTENDED_COMMENT;
                        break;
                    case '*':
                        if (d == '/') {
                            curr = REGULAR_CODE;
                        }
                        break;
                }
                
            case SINGLE_LINE_JAVADOC:
                switch(c) {
                    case ' ':
                        curr = SINGLE_LINE_LEADING_CHARACTERS;
                        break;
                    case NEW_LINE_ASCII:
                        curr = REGULAR_CODE;
                        break;
                }
        
            case SINGLE_LINE:
                switch(c) {
                    case NEW_LINE_ASCII:
                        putchar(NEW_LINE_ASCII);
                        curr = REGULAR_CODE;
                        break;    
                    case BACKSLASH_ASCII:
                        getchar();
                        break;
                    case ' ':
                        if(d == '@') {
                            curr = SINGLE_LINE_JAVADOC;
                        } else {
                            putchar(c);
                        }
                    default:
                        putchar(c);
                        break;
                }
        }
        
    }
    return 0;
}

// from Aspnes notes
int peekchar(void) {
    int c; // the declaration
    
    c = getchar(); // read in the first char
    if(c != EOF) { // if it's not the end of file
        ungetc(c, stdin); // put it back lol
    }
    
    return c;
}