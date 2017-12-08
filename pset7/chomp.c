#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "chomp.h"

char **chomp_states(int rows, int cols, int *n)
{
  if (rows <= 0 || cols <= 0 || rows > 9)
    {
      return NULL;
    }

  // count[i][j] is the number of rows*(i+1) chomp states with exactly j in the last column
  int count[cols + 1][rows + 1];
  for (int r = 0; r <= rows; r++)
    {
      count[0][r] = 1;
    }

  for (int c = 2; c <= cols + 1; c++)
    {
      for (int r = 0; r <= rows; r++)
	{
	  int sum = 0;
	  for (int r2 = r; r2 <= rows; r2++)
	    {
	      sum += count[c - 2][r2];
	      if (sum <= 0)
		{
		  // overflow
		  return NULL;
		}
	    }
	  count[c - 1][r] = sum;
	}
    }

  *n = count[cols][0];

  char **result = malloc(sizeof(char *) * *n);
  if (result != NULL)
    {
      // last state is whole pan
      result[*n - 1] = malloc(sizeof(char) * (cols + 1));
      if (result[*n - 1] != NULL) {
		  for (int i = 0; i < cols; i++) {
		      result[*n - 1][i] = '0' + rows;
		    }
	  result[*n - 1][cols] = '\0';
	}

      // each previous state is obtained by decrementing the next
      for (int i = *n - 2; i >= 0; i--) {
		  if (result[i + 1] != NULL) {
		  	
		  	// malloc space for the new string
		      result[i] = malloc(sizeof(char) * (cols + 1));
		      if (result[i] != NULL) {
		      	
		      	// copy the string ahead to the space of the new string
				  strcpy(result[i], result[i + 1]);
				  
				  // find last non-zero (know there will be one)
				  int j = cols - 1;
				  while (result[i][j] == '0') {
				      j--;
				   }
				  
			  // decrement it
			  result[i][j]--;
	
			  // set all subsequent to equal it
			  for (int k = j + 1; k < cols; k++) {
			      result[i][k] = result[i][j];
		    	}
			} // end of if for malloc'ing the string 
	    } // end of if for mallocing the base case
	} // end of for loop to write everything
    } else {
      *n = 0;
    }

  return result;
}

// #define CSDBG
char **chomp_successors(const char *s, int *n) {
	
	if(s != NULL) {
		
		// get the number of cols
		int ncols = strlen(s);
		
		// how many possible states will there be? the sum of the cols in s.
		int nstates = 0;
		for(int i = 0; i < ncols; i++) {
			int c = s[i] - '0';
			nstates += c;
		}
		*n = nstates;
	
		#ifdef CSDBG
		printf("nstates: %d\n", nstates);
		#endif
		
		// create the array to store the states
		// first, we allocate the array of pointers
		char ** states = malloc(sizeof(char *) * (nstates));
		if(states != NULL) {
			
			int index = 0;
			
			// for each column
			for(int col = 0; col < ncols; col++) {
				
				// get the height at that column
				int height = s[col] - '0';
				
				#ifdef CSDBG
				printf("column in question: %d\n", col);
				printf("height of column in question: %d\n", height);
				#endif
				
				// for each possible height removal from that column
				for(int h = 0; h < height; h++) {
					
					#ifdef CSDBG
					printf("h: %d\n", h);
					#endif
					
					// create a string to store in that index
					states[index] = malloc( sizeof(char) * (ncols+1) );
					if(states[index] != NULL) {
						
						// for each 'tower', where a tower is another dummy for a height of a column
						// this is the string processing component
						
						/** okay so the reason why we can't simply set tower to col here is because
						 * we need to give values for the previous sides. This, however, can be done
						 * with a for loop to preprocess the strings.
						 */ 
						 
						char * state = states[index];
						for(int i = 0; i < col; i++) {
							state[i] = s[i];
						}
						
						for(int tower = col; tower < ncols; tower++) {
							
							#ifdef CSDBG
							printf("tower: %d\n", tower);
							#endif
							
							int tower_height = s[tower] - '0';
							
							
							#ifdef CSDBG
							printf("tower height: %d\n", tower_height);
							#endif
							
							// if the height of the tower is greater than h, cut off the top.
							if(tower_height >= h) {
								state[tower] = h + '0';
							} 
							
							// else, copy it as it is.
							else {
								state[tower] = s[tower];
							}
						} // end of for
						state[ncols] = '\0';
					} // end of if to check that index alloc worked

					index++;
				}
			}
		}
		
		return states;
	} else {
		return NULL;
	}
}

#define MAIN
#ifdef MAIN
int main(int argc, char **argv) {

	int n = 0;
	char ** states = chomp_successors("54311", &n);
	
	printf("n: %d\n", n);
	
	for(int i = 0; i < n; i++) {
		printf("(%d) possible: %s\n", i, states[i]);
		free(states[i]);
	}
	free(states);

	return 0;
}
#endif
