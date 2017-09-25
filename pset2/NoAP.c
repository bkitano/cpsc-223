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
    int sq_index = 0;
    
    // the first values are the first must includes.
    for(int i = 0; i < must_includes_length; i++) {
        sequence[i] = must_includes[i];
        sq_index++;
    }
    
    
    for(int guess = must_includes[must_includes_length-1] + 1; guess < range; guess ++) {
        // iterate through the guesses
        bool has_arithmetic = false;
        printf("guess: %d\n", guess);
        
        for(int seq1 = 0; seq1 < sq_index - 1; seq1 ++) {
            // the furthest seq val
            int seq1_val = sequence[seq1];
            
            for (int seq2 = seq1 + 1; seq2 < sq_index; seq2 ++) {
                int seq2_val = sequence[seq2];
                
                int test[] = {seq1_val, seq2_val, guess};
                has_arithmetic = has_arithmetic || arithmetic(test);
            }
        }
        printf("guess: %d | has_arithmetic: %d\n", guess, has_arithmetic);
        if(!has_arithmetic) {
            sequence[sq_index] = guess;
            sq_index ++;
        }
    }
    
    
    // print the shit correctly
    printf("-greedy: %d [", sq_index + 1);
    for(int i = 0 ; i < sq_index - 1; i++) {
        printf("%d, ", sequence[i]);
    } // end of for
    printf("%d]\n", sequence[sq_index-1]);

    
    
} // end of greedy
    
    


void swap(int *xp, int *yp) {
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// bubble sort
// adapted from http://www.geeksforgeeks.org/bubble-sort/
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
    bool answer;
    bubble_sort(nums, 3);
    printf("sorted: %d %d %d \n", nums[0], nums[1], nums[2]);
    double sum = 0;
    double length = 3;

    for (int i = 0; i < length; i ++) {
        sum += nums[i];
    }
    printf("sum: %fl \n", sum);
    printf("middle number: %d \n", nums[1]);
    
    printf("sum/length: %fl \n", sum / length);
    if(sum/length == nums[1]) {
        answer = true;
    } else {
        answer = false;
    }
    printf("arithmetic: %d \n\n", answer);
    return(answer);
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

