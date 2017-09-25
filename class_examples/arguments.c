#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>

bool is_all_digits(char *s);
int average(int n, char *a[]);

int main(int argc, char **argv)
{
  int tot = 0;
  printf("Welcome to %s!\n", 0[argv]); // 0[argv] = *(0 + argv) = *(argv + 0) = argv[0]

  for (int i = 1; i < argc; i++)
    {
      if (is_all_digits(argv[i]))
	{
	  tot += atoi(argv[i]);
	}
      printf("%s\n", argv[i]);
    }
  printf("sum of positive integer arguments = %d\n", tot);
  printf("average of positive integer arguments = %d\n", average(argc - 1, argv + 1));
  
  /*
  char nums[][4] = {"123", "456", "789"};
  printf("%x\n", nums[0]);
  printf("%x\n", nums[1]);
  printf("%x\n", nums[2]);
  printf("%d\n", average(3, nums)); // char[][] is not the same as char **; this compiles with a warning and crashes when executed
  */

}

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

int average(int n, char *a[])
{
  int tot = 0;
  int count = 0;
  for (int i = 0; i < n; i++)
    {
      if (is_all_digits(a[i]))
	{
	  tot += atoi(a[i]);
	  count++;
	}
    }
  if (count > 0)
    {
      return tot / count;
    }
  else
    {
      return INT_MAX;
    }
}
