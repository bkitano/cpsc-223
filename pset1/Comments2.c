#include <stdio.h>
#include <stdlib.h>

#define SINGLE_QUOTE_ASCII 39
#define DOUBLE_QUOTE_ASCII 34
#define NEW_LINE_ASCII 10
#define BACKSLASH_ASCII 92
#define FORWARDSLASH_ASCII 47

typedef enum {STRING_LITERAL_SUPPRESS, ONE_SLASH, LEADING_STAR, EXTENDED_COMMENT,
    JAVADOC, TRAILING_STAR, SINGLE_LINE,
    REGULAR_CODE} state;

int main() {
    
    int c; // the character being read
    state curr = REGULAR_CODE; // initialize the state at REGULAR_CODE
        
    while ( (c = getchar()) != EOF) {
        
        switch (curr) {
            
            case REGULAR_CODE:
                switch(c) {
                    case FORWARDSLASH_ASCII:
                        curr = ONE_SLASH;
                        break;
                    case DOUBLE_QUOTE_ASCII:
                        curr = STRING_LITERAL_SUPPRESS;
                        break;
                }
                break;
        
            case STRING_LITERAL_SUPPRESS:
                if(c == DOUBLE_QUOTE_ASCII) {
                    curr = REGULAR_CODE;
                }
                break;
        
            case ONE_SLASH:
                switch(c) {
                    case '*':
                        curr = LEADING_STAR;
                        break;
                    case FORWARDSLASH_ASCII:
                        curr = SINGLE_LINE;
                        break;
                    default:
                        curr = REGULAR_CODE;
                }
                break;
        
            case LEADING_STAR:
                switch(c) {
                    case '*':
                        curr = LEADING_STAR;
                        break;
                    case ' ':
                        curr = LEADING_STAR;
                        break;
                    default:
                        putchar(c);
                        curr = EXTENDED_COMMENT;
                        break;
                }
                break;
        
            case EXTENDED_COMMENT:
                switch(c) {
                    case '@':
                        curr = JAVADOC;
                        break;
                    case '*':
                        curr = TRAILING_STAR;
                        break;
                    case BACKSLASH_ASCII:
                        getchar();
                        break;
                    default:
                        putchar(c);
                        break;
                }
                break;
        
            case TRAILING_STAR:
                switch (c) {
                    case '*':
                        curr = TRAILING_STAR;
                        break;
                    case FORWARDSLASH_ASCII:
                        putchar(NEW_LINE_ASCII);
                        curr = REGULAR_CODE;
                        break;
                    default:
                        curr = EXTENDED_COMMENT;
                        break;
                }
                break;    
        
            case JAVADOC:
                if(c == ' ') {
                    curr = EXTENDED_COMMENT;
                } else {
                    curr = JAVADOC;
                }
                break;    
        
            case SINGLE_LINE:
                switch(c) {
                    case NEW_LINE_ASCII:
                        putchar(NEW_LINE_ASCII);
                        curr = REGULAR_CODE;
                        break;    
                    case BACKSLASH_ASCII:
                        getchar();
                        break;
                    default:
                        putchar(c);
                        break;
                }
        }
        
    }
    
    
    return 0;
}