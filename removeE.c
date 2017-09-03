/*not functional code*/

#include <stdio.h>

int main(int argc, char **argv) {
    int c;
    
    while( (c = getchar()) != EOF) {
        switch(c) {
            case 'e':
            case 'E':
                break;
            default:
                putchar(c);
                break;
        }
    }
    
    return 0;
}