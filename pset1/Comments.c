#include <stdio.h>
#include <stdlib.h>

#define SINGLE_QUOTE 39
#define DOUBLE_QUOTE 34
#define NEW_LINE 10

int peekchar(void);
void reset_all_flags(void);

int main(int argc, char **argv) {
    
    int c; // the current character being read from stdin
    
    // the comment toggles
    int line_comment = 0;
    int extended_comment = 0;
    int extended_comment_delimiter = 0;
    int line_comment_delimiter = 0;
    int double_quote = 0;

    
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
            if(c == NEW_LINE) {
                line_comment = 0;
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
            } else if (c == NEW_LINE) {
                putchar(c);
                getchar();
            }
            else {
                putchar(c);
            }
        }
        
        if(double_quote) {
            if(c == DOUBLE_QUOTE) {
                double_quote = 0;
            }
            getchar();
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
        
        if(c == DOUBLE_QUOTE) {
            reset_all_flags();
            double_quote = 1;
        }
        
    }
    return 0;
}

void reset_all_flags(void) {
    line_comment = 0;
    extended_comment = 0;
    extended_comment_delimiter = 0;
    line_comment_delimiter = 0;
    double_quote = 0;
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

/*
test2: still outputing a star
test4: ahh not sure but def fucked up
test8: need to remove @ shit
test9: failing the string literal test
test10: the beginning of a comment should be treated as the beginning of a line

Need to be consistent about the procedure for ending flags
*/