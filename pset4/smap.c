#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "smap.h"

#define SMAP_INITIAL_CAPACITY 3

/**
 * An implementation of a map of strings as an unsorted fixed-size array.
 * This implementation will not meet the requirements of Assignment #4 and
 * not completely tested itself but should be sufficient for testing your
 * Cooccurrence matrix.
 */

typedef struct
{
  char * key; // a string
  int * value; // an array of ints for each line in the corpus
  bool filled;
} entry;

struct smap
{
  int capacity;
  int size;
  int (*hash)(const char *);
  entry *table;
};

int smap_table_find_key(const entry *table, const char *key, int (*hash)(const char *), int size, int capacity);
void smap_embiggen(smap *m);


// runs in O(1)
smap *smap_create(int (*h)(const char *s))
{
  smap *result = malloc(sizeof(smap));
  if (result != NULL)
    {
      result->size = 0;
      result->hash = h;
      result->table = malloc(sizeof(entry) * SMAP_INITIAL_CAPACITY);
      for(int i = 0; i < SMAP_INITIAL_CAPACITY; i++) {
        result->table[i].filled = false;
      }
      result->capacity = (result->table != NULL ? SMAP_INITIAL_CAPACITY : 0);
    }
  return result;
}

// O(1)
int smap_size(const smap *m)
{
  return m->size;
}

/**
 * Returns the index where key is located in the given map, or the index
 * where it would go if it is not present.
 * 
 * @param table a table with at least one free slot
 * @param key a string, non-NULL
 * @param hash a hash function for strings
 * @param size the size of the table
 * @param capacity the capacity of the table
 * @return the index of key in table, or the index of the empty slot to put it in if it is not present
 */

// TESTED, working as of 2219 LEAKS
int smap_table_find_key(const entry *table, const char *key, int (*hash)(const char *), int size, int capacity) {
  // sequential search for key
  int index = hash(key) % capacity;
  
  if(table[index].filled) {
    
    // if the key is in that table
    if(!strcmp( table[index].key, key )) {
      return index;
    } else { 
      
      // there's a collision
      // keep going until you find an open address
      
      int test = index;
      while (table[test].filled == true) {
        
        // if you've found the key
        if(!strcmp(table[test].key, key)) {
          return test;
        }
        
        // else increment
        test = (test + 1) % capacity;
      }
      
      // test is now the first open address, so return it.
      return test;
    }
  } else {
    // the index you hashed to is open, so nice.
    return index;
  }
  
    return index;
}

bool smap_put(smap *m, const char *key, int *value) {
  
  if (m != NULL) {
    
      if (m->size == m->capacity / 2) {
    	  smap_embiggen(m);
    	}
    
      int index = smap_table_find_key(m->table, key, m->hash, m->size, m->capacity);
      
      if(m->table[index].filled) {
    	  // key already present
    	  m->table[index].value = value;
    	  return true;
	    } else {
	      
    	  // new key; check whether there is room for the new entry
    	 // if (m->size == m->capacity / 2) {
  	   //   // still no room :(
  	   //   return false;
	     // }
	  
    	  // make a copy of the new key
    	  char *copy = malloc(strlen(key) + 1);
    	  strcpy(copy, key);
  	  
    	  if (copy != NULL) {
  	      m->table[index].key = copy;
  	      m->table[index].value = value;
  	      m->table[index].filled = true;
  	      m->size++;
  	      return true;
  	    } else {
  	      return false;
  	    }
  	}
  } else {
    // NULL matrix
    return false;
  }
}

void smap_embiggen(smap *m) {
  
  // double the capacity, make a temp table of that size
  int larger_capacity = 2 * m->capacity;
  int old_capacity = m->capacity;
  m->capacity = larger_capacity;
  
  // create a new, larger table
  entry * larger_table = (entry *) malloc(sizeof(entry) * larger_capacity);
  
  // create a way to reference the old table
  entry * old_table = m->table;

  // switch the table over to the empty new table
  m->table = larger_table;
  m->size = 0;
  
  // set the filled value for all the new entries to false (prevents invalid reads)
  for(int new_index = 0; new_index < larger_capacity; new_index ++) {
    m->table[new_index].filled = false;
  }
  
  // rehash and reindex every value in the old table
  for (int old_index = 0; old_index < old_capacity; old_index++) {
    
    // if there was an entry here BAD READ
    if(old_table[old_index].filled) {
      
      // put it in the correct spot
      smap_put(m, old_table[old_index].key, old_table[old_index].value);
      free(old_table[old_index].key);
    }
  }  

  free(old_table);
}

bool smap_contains_key(const smap *m, const char *key) {

  // if find_key is empty, then 
  int supposed_index = smap_table_find_key(m->table, key, m->hash, m->size, m->capacity);
  if(m->table[supposed_index].filled) {
    return true;
  } else {
    return false;
  }
  
}

int *smap_get(smap *m, const char *key) {
  
  int index = smap_table_find_key(m->table, key, m->hash, m->size, m->capacity);
  
  //LEAK
  if ( m->table[index].filled ) {
    return m->table[index].value;
  } else {
    return NULL;
  }
}

void smap_for_each(smap *m, void (*f)(const char *, int *)) {
  for (int i = 0; i < m->capacity; i++) {
    if(m->table[i].filled) {
      f(m->table[i].key, m->table[i].value);
    }
  }
} 

void smap_for_each_r(smap *m, void (*f)(const char *, int *, void *), void *arg) {
  for (int i = 0; i < m->capacity; i++) {
    if(m->table[i].filled) {
      f(m->table[i].key, m->table[i].value, arg);
    }
  }
}  

void smap_destroy(smap *m) {
  // free all the copies of the keys we made
  for (int i = 0; i < m->capacity; i++) {
    if(m->table[i].filled) {
      free(m->table[i].key);
    }
  }

  // free the table
  free(m->table);

  // free the smap struct
  free(m);
}

/*
int hash(const char * word) {
    
    int l = strlen(word);
    
    // forces the hash to run in constant time
    if(l > 200) {
        l = 200;
    }
    
    int sum = 0;
    for(int i = 0; i < l; i++) {
        sum += word[i];
    }
    
    return (38183 * sum);
}


int main(int argc, char **argv) {
  smap * m = smap_create(hash);
  
  for (int i = 1; i < argc; i++) {
    
    int * j = malloc(sizeof(int));
    *j = i;
    smap_put(m, argv[i], j);
    
  }
  
  // clearing
  printf("%d\n", smap_size(m));

  return 1;
}
*/

