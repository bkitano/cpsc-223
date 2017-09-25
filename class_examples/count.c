/* Headers are like packages or modules in Python. Apparently, always include
stdio or else your code might not compile. */
#include <stdio.h>

/*Every C code needs a main function declared in exactly this same way.*/
int main(int argc, char **argv) {
    
    int i;
    
    puts("Now I will count from 1 to 10");
    
    for(i=0; i<10; i++) {
        printf("%d\n", i);
    }
    
    return 0;
}