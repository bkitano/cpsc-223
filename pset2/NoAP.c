#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

bool is_all_digits(char *s);

int main(int argc, char **argv) {
    
    // first argument is the size
    // int n = atoi(argv[1]);
    
    int i;
    for (i = 1; i < argc; i++) {
        // printf("%s \n", argv[i]);
        
        if (is_all_digits(argv[i])) {
            printf("Digits: %s \n", argv[i]);
        } else {
            printf("Non numbers: %s \n", argv[i]);
        }
    }
    
    // the next arguments are the must-haves
    
    
    // the last arguments are the methods
    
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