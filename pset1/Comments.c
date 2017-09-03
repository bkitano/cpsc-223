#include <stdio.h>
#include <stdlib.h>

int peekchar(void);

int main(int argc, char **argv) {
    
    int c; // the current character being read from stdin
    
    // the comment toggles
    int line_comment = 0;
    int extended_comment = 0;
    int extended_comment_delimiter = 0;
    int line_comment_delimiter = 0;

    
    while( (c = getchar()) != EOF) {
        
        // c is the current char, d is the next char
        int d = peekchar();
        
        if(extended_comment_delimiter) {
            if (c != ' ' && c != '*') {
                extended_comment_delimiter = 0;
                extended_comment = !extended_comment;
            }
        }
        
        if(line_comment_delimiter) {
            if (c != ' ' && c !='/') {
                line_comment_delimiter = 0;
                line_comment = !line_comment;
            }
        }
        
        // if you're in a line comment, print until a new line 
        if(line_comment) {
            if(c == 10) {
                line_comment = 0;
                // putchar('\n');
            } else {
                putchar(c);            
            }    
        }
        
        // if you're in an extended comment, print until the tags close
        if(extended_comment) {
            
            // switch this logic
            if(c == '*') {
                // switch up the logic haha i crack myself up
                switch (d) {
                    case '/':
                        extended_comment_delimiter = 1;
                        break;
                    case '*':
                        extended_comment_delimiter = 1;
                        break;
                    case ' ':
                        getchar();
                        break;
                    default:
                        putchar(c);
                }
            } else if (c == 10) {
                putchar(c);
                getchar();
            }
            else {
                putchar(c);
            }
        }
        
        // check if you've entered a line comment
        if(c == '/' && d == '/') {
            line_comment_delimiter = 1;
        }
        
        // check if you've entered an extended comment, skip the next two 
        // characters, since they're the delimiters
        if(c == '/' && d == '*') {
            extended_comment_delimiter = 1;
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

