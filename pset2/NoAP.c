#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

bool is_all_digits(char *s);
void swap(int *xp, int *yp);
void bubble_sort(int nums[], int n);
bool arithmetic(int nums[]);
void greedy(int range, int must_includes[], int must_includes_length);

int main(int argc, char **argv) {
    
    // first argument is the size
    char *n = argv[1];
    int range;

    // error checking for n:
    // if n not positive, error message
    if(!is_all_digits(n)) {
        printf("NoAP: n must not be negative; was value %s \n", n);
        return 1;
    } else {
        range = atoi(n);
        // printf("Range: %d \n", range);
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

    // error checking for must_includes:
    // if any are not in the range 0:(n-1), display error message

    for (int k = 0; k < must_includes_length; k++) {
        if( must_includes[k] > (range-1) ) {
            printf("NoAP: integer out of range %d \n", must_includes[k]);
            return 1;
        }
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
    
    // end of preprocessing
    // start of algorithms
    
    int test[] = {2,4,8};
    greedy(15, test, 3);
    
    return 0;
}

// implement greedy
void greedy(int range, int must_includes[], int must_includes_length){
    
    // instantiate an empty int array to store the sequence
    int sequence[range];
    
    // fill it with nonsense
    for(int i = 0; i < range; i++) {
        sequence[i] = -1;
    }
    
    // counters that will track where in the arrays we are up to
    int mi_index = 0;
    int sq_index = 0;
    
    // the first value in the array is the first value in must_includes
    sequence[0] = must_includes[0];
    mi_index ++;
    sq_index ++;
    
    // if the next number in must_includes == sequence[0] + 1, append it
    while (mi_index < must_includes_length) { // while there are still values in must_includes
        if(must_includes[mi_index] == sequence[sq_index - 1] + 1) {
            // if the current val of must_includes is the current val of sq + 1, 
            // there will be no space for an arithmetic, so you must append it.
            sequence[sq_index] = must_includes[mi_index];
            sq_index ++;
            mi_index ++;
        } else {
            // so there's a gap between the current sequence index and the next must_include
            // the answer will be inside this range

            // use while loops
            for( int guess = sequence[sq_index-1] + 1; guess < must_includes[mi_index]; guess++) {
                
                // tally over all permutations whether there is an arithmetic progression
                bool guess_has_arithmetic = false;
                
                // for all sequence vals before the current one
                for (int ext = 0; ext < sq_index; ext ++) {
                    
                    int sq_extant = sequence[ext];
                    
                    // for all must includes greater than the guess
                    for (int fut = mi_index; fut < must_includes_length; fut++) {
                        
                        int future_vals = must_includes[fut];
                        
                        int test[] = {guess, sq_extant, future_vals};
                        guess_has_arithmetic = guess_has_arithmetic || arithmetic(test);
                    }
                }
                // if the number doesn't have an arithmetic sequence, set the val up
                if (!guess_has_arithmetic) {
                    sequence[sq_index] = guess;
                    sq_index ++;
                }
            }
            sequence[sq_index] = must_includes[mi_index];
        }
        mi_index ++;
    }
    // okay, now we have to handle everything beyond the included values until the range
    printf("mi_index: %d \n", mi_index);
    printf("sq_index: %d \n", sq_index);
    
    // at the end, print all the values
    printf("-greedy: %d [", range);
    for (int i = 0; i < range-1; i++) {
        printf("%d, ", sequence[i]);
    }
    printf("%d]\n", sequence[range-1]);
}




void swap(int *xp, int *yp) {
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// bubble sort
void bubble_sort(int nums[], int n) {
    int i,j;
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++) {
            if( nums[j] > nums[j+1]) {
                swap(&nums[j], &nums[j+1]);
            }
        }
    }
}

bool arithmetic(int nums[]) {
    bubble_sort(nums, 3);
    printf("sorted: %d %d %d \n", nums[0], nums[1], nums[2]);
    int sum = 0;
    int length = 3;

    for (int i = 0; i < length; i ++) {
        sum += nums[i];
    }
    printf("sum: %d \n", sum);
    printf("middle number: %d \n", nums[1]);
    
    if(sum/length == nums[1]) {
        return true;
    } else {
        return false;
    }
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

