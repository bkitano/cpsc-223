#include <stdio.h>
#include <stdlib.h>

#define SINGLE_QUOTE_ASCII 39
#define DOUBLE_QUOTE_ASCII 34
#define NEW_LINE_ASCII 10
#define BACKSLASH_ASCII 92
#define FORWARDSLASH_ASCII 47

typedef enum {
REGULAR_CODE,
ONE_SLASH,
EXTENDED_LEADING_SPACE,
SINGLE_LEADING_SPACE,
STRING_LITERAL,
EXTENDED_JAVADOC,
EXTENDED_COMMENT,
SINGLE_LINE_COMMENT,
SINGLE_JAVADOC
} state;

int peekchar(void);

int main() {
    
    int c;
    state curr = REGULAR_CODE;
    
    while( (c = getchar()) != EOF) {
        
        int d = peekchar();
        
        switch(curr) {
            
            case REGULAR_CODE:
                switch(c) {
                    case FORWARDSLASH_ASCII:
                        curr = ONE_SLASH;
                        break;
                    case DOUBLE_QUOTE_ASCII:
                        curr = STRING_LITERAL;
                }
                break;
                
            case ONE_SLASH:
                switch(c) {
                    case '*':
                        curr = EXTENDED_LEADING_SPACE;
                        break;
                    case '/':
                        curr = SINGLE_LEADING_SPACE;
                        break;
                    default:
                        curr = REGULAR_CODE;
                }
                break;
                
            case SINGLE_LEADING_SPACE:
                switch(c) {
                    case ' ':
                        if(d == '@') {
                            curr = SINGLE_JAVADOC;
                        } else {
                            curr = SINGLE_LEADING_SPACE;
                        }
                        break;
                    default:
                        putchar(c);
                        curr = SINGLE_LINE_COMMENT;
                        break;
                }
                break;
            
            case SINGLE_LINE_COMMENT:
                switch(c) {
                    case '\n':
                        putchar(c);
                        curr = REGULAR_CODE;
                        break;
                    case ' ':
                        putchar(c);
                        if (d == '@') {
                            curr = SINGLE_JAVADOC;
                        }
                        break;
                    default:
                        putchar(c);
                        break;
                }
                break;
                
            case STRING_LITERAL:
                switch(c) {
                    case DOUBLE_QUOTE_ASCII:
                        putchar(NEW_LINE_ASCII);
                        curr = REGULAR_CODE;
                    }
                break;
            
            case EXTENDED_LEADING_SPACE:
                switch(c) {
                    case '*':
                        if (d == '/') {
                            curr = REGULAR_CODE;
                        } else {
                            curr = EXTENDED_LEADING_SPACE;
                        }
                        break;
                    case ' ':
                        if(d == '@') {
                            curr = EXTENDED_JAVADOC;
                        } else {
                        curr = EXTENDED_LEADING_SPACE;
                        }
                        break;
                    case '\n':
                        curr = EXTENDED_LEADING_SPACE;
                        break;
                    default:
                        putchar(c);
                        curr = EXTENDED_COMMENT;
                        break;
                }
                break;
                
            case EXTENDED_COMMENT:
                switch (c) {
                    
                    case '*':
                        switch(d) {
                            case '/':
                                putchar(NEW_LINE_ASCII);
                                curr = REGULAR_CODE;
                                break;
                            default:
                                putchar(c);
                        }
                        break;
                    
                    case '\n':
                        putchar(c);
                        curr = EXTENDED_LEADING_SPACE;
                        break;
                        
                    case BACKSLASH_ASCII:
                        getchar();
                        getchar();
                        
                    case ' ':
                        putchar(c);
                        if(d == '@') {
                            curr = EXTENDED_JAVADOC;
                        }
                        break;
                    
                    default:
                        putchar(c);
                        break;
                } // extended comment
                break;
                
            case EXTENDED_JAVADOC:
                switch(c) {
                    case ' ':
                        curr = EXTENDED_LEADING_SPACE;
                        break;
                }
                break;
            
            case SINGLE_JAVADOC:
                switch(c) {
                    case ' ':
                        curr = SINGLE_LEADING_SPACE;
                        break;
                }
                break;
                
            
            
        } // switch curr
    } // end of while
        
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