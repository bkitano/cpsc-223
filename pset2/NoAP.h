#ifndef __NO_AP__
#define __NO_AP__

bool is_all_digits(char *s);
void swap(int *xp, int *yp);
void bubble_sort(int nums[], int n);
bool arithmetic(int nums[]);
void greedy(int range, int must_includes[], int must_includes_length, bool must_includes_check);
void backward(int range, int must_includes[], int must_includes_length, bool must_includes_check);
void skip(int range, int must_includes[], int must_includes_length, int first, int step);
bool is_in(int array[], int size, int number);


#endif