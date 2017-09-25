#include <stdio.h>
#include <stdlib.h>

#define FILENAME "hello.txt"
#define MESSAGE "hello world"

int main(int argc, char **argv) {
    FILE *f;
    
    // open the file
    f = fopen(FILENAME, "w");
    
    // if there's no file, exit
    if(f==0){
        perror(FILENAME);
        exit(1);
    }
    
    // write MESSAGE to file f
    fputs(MESSAGE, f);
    putc('\n', f);
    
    // close the file
    fclose(f);
    
    return 0;
}