#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

bool is_all_digits(char *s);

int main(int argc, char **argv) {
    
    // first argument is the size
    char *n = argv[1];
    int range;

    // error checking for n:
    // if n not positive, error message
    if(!is_all_digits(n)) {
        printf("NoAP: n must not be negative; was value %s \n", n);
    } else {
        range = atoi(n);
        printf("Range: %d \n", range);
    }
    
    // the next arguments are the must-haves
    // logic: have an array of length argc and fill it with garbage
    // then replace those values.
    
    int i = 2;
    while(is_all_digits(argv[i])) {
        i++;
    }
    int must_includes_length = i-2;
    
    int must_includes[must_includes_length];
    
    if( must_includes_length != 0) {
        for (int j = 2; j < must_includes_length + 2; j++) {
            must_includes[j-2] = atoi(argv[j]);
        }
    }
    
    for (int k = 0; k < must_includes_length; k ++) {
        printf("must_includes[%d]: %d \n", k, must_includes[k]);
    }
    
    // the last arguments are the methods
    // we know that the next arguments are flags
    int flags_length = argc - 2 - must_includes_length; 
    char * flags[flags_length];
    int k = must_includes_length + 2;
    
    while(k < argc) {
        flags[k - must_includes_length - 2] = argv[k];
        k++;
    }
    
    for(int i = 0; i < flags_length; i ++) {
        printf("flag[%d]: %s \n", i, flags[i]);
    }
    
    
    return 0;
}

// copied from class example arguments.c
bool is_all_digits(char *s)
{
  while (isdigit(*s))
    {
      s++;
    };

  if (*s == '\0')
    {
      return true;
    }
  else
    {
      return false;
    }
}