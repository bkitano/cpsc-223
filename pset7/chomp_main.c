#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "chomp.h"
#include "string_helpers.h"
#include "smap.h"

enum {CURRENT, OTHER};

// #define dbg
int main(int argc, char **argv) {
  
  int WIN = 1;
  int LOSS = 0;
  
  // error checking on input
  if (argc < 2 || strlen(argv[1]) == 0 || argv[1][0] == '0') {
    fprintf(stderr, "USAGE: %s state\n", argv[0]);
    return 1;
  }

  int cols = strlen(argv[1]);
  int rows = 1;
  for (char *s = argv[1]; *s; s++) {
    if (!isdigit(*s) || *s < *(s + 1)) {
  	  // string contains non-digit or increasing digits
  	  fprintf(stderr, "USAGE: %s state\n", argv[0]);
  	  return 1;
  	}

    // rows is max over all digits in string
    int h = *s - '0';
    if (h > rows) {
  	  rows = h;
  	}
  }

  // gets all possible states for the board
  int apcount;
  char **states = chomp_states(rows, cols, &apcount); // free'd at 136
  if (states == NULL) {
    fprintf(stderr, "%s: error allocating memory\n", argv[0]);
    return 1;
  }
  
  // create a map from states (strings) to position values
  smap * state_to_pos = smap_create(string_hash_java); // free'd at 
  
  // set final state to win for current player
  
  // create final state
  char final_state[cols + 1];
  for(int i = 0; i < cols; i++) {
    final_state[i] = '0';
  }
  final_state[cols] = '\0';
  
  // works
  smap_put(state_to_pos, final_state, &WIN);
  
  // for each other state
  for (int i = 1; i < apcount; i++) {
    
    // get successor states
    int succ_count;
    char **succ = chomp_successors(states[i], &succ_count);
    
    if (succ != NULL) {
      
	  // if all successor states are wins then set states[i] to loss
	  // else set states[i] to win
	  
  	  int j = 0;
  	 // int * outcome = smap_get(state_to_pos, succ[0]);
  	  
  	  // while the outcomes are all wins
  	  while (j < succ_count && smap_get(state_to_pos, succ[j]) != NULL && *(smap_get(state_to_pos, succ[j])) != LOSS) {
  	   // printf("succ[%d]: %s | outcome: %d\n", j, succ[j], *outcome );
  	    
  	    j++;
  	   // if(j < succ_count) { // to prevent seg faults
  	   //   outcome = smap_get(state_to_pos, succ[j]); // will return null if it's not there
  	   // }
  	    
  	  } // end of while 
  	  
  	  // if we've hit a loss, then we insert it as a win
  	  if(j < succ_count) {
  	    smap_put(state_to_pos, states[i], &WIN);
  	  } 
  	  // else outcome is a WIN, so it iterated through everything.
  	  else {
  	    smap_put(state_to_pos, states[i], &LOSS);
  	  }

	  } // end of if checking succ != NULL
	  
    string_array_free(succ, succ_count);
  } // end of for iterating through all possible trays

  // look up result for argv[1]
  int * result = smap_get(state_to_pos, argv[1]);
  
  // if a loss then print LOSS
  if (*result == LOSS) {
    printf("LOSS\n");
  } else {
    
    // get the successors
    int nsuccs;
    char ** nsucc = chomp_successors(argv[1], &nsuccs);
    if(nsucc == NULL) {
      return 1;
    }
    
    // string_array_fprintf(stdout, nsucc, nsuccs);
    
    int t = 0;
    char * next = nsucc[t];
    
    int * temp = smap_get(state_to_pos, nsucc[t]);
    
    // keep iterating until we find a win
    while(t < nsuccs  && temp != NULL) {
      
      if(*temp == LOSS) {
        printf("WIN: %s\n", next);
        string_array_free(nsucc, nsuccs);
        smap_destroy(state_to_pos);
        string_array_free(states, apcount);
        return 0;
      }
      
      t++;
      next = nsucc[t];
      temp = smap_get(state_to_pos, nsucc[t]);  
    }
    
    string_array_free(nsucc, nsuccs);
  }
  
  
  // else find first successor that is a win and output WIN: that successor
  
  smap_destroy(state_to_pos);
  string_array_free(states, apcount);
}
