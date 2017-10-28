#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "smap.h"

/**
 * An implementation of a map of strings as an unsorted fixed-size array.
 * This implementation will not meet the requirements of Assignment #4 and
 * not completely tested itself but should be sufficient for testing your
 * Cooccurrence matrix.
 */

typedef struct
{
  char *key;
  int* value;
} entry;

struct smap
{
  int capacity;
  int size;
  int (*hash)(const char *);
  entry *table;
};

#define SMAP_INITIAL_CAPACITY 100

int smap_table_find_key(const entry *table, const char *key, int (*hash)(const char *), int size, int capacity);
void smap_embiggen(smap *m);

smap *smap_create(int (*h)(const char *s))
{
  smap *result = malloc(sizeof(smap));
  if (result != NULL)
    {
      result->size = 0;
      result->hash = h;
      result->table = malloc(sizeof(entry) * SMAP_INITIAL_CAPACITY);
      result->capacity = (result->table != NULL ? SMAP_INITIAL_CAPACITY : 0);
    }
  return result;
}

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
int smap_table_find_key(const entry *table, const char *key, int (*hash)(const char *), int size, int capacity)
{
  // hash the key, get the index
  int i = hash(key);
  return i;
}

bool smap_put(smap *m, const char *key, int *value)
{
  if (m != NULL)
    {
      if (m->size == m->capacity)
	{
	  // table is full (would that be the right condition for a hash table?); try to expand
	  smap_embiggen(m); // does nothing in this implementation
	}

      // find key
      int index = smap_table_find_key(m->table, key, m->hash, m->size, m->capacity);
      if (index < m->size)
	{
	  // key already present
	  m->table[index].value = value;
	  return true;
	}
      else
	{
	  // new key; check whether there is room for the new entry
	  if (m->size == m->capacity)
	    {
	      // still no room :(
	      return false;
	    }
	  
	  // make a copy of the new key
	  char *copy = malloc(strlen(key) + 1);
	  strcpy(copy, key);
	  
	  if (copy != NULL)
	    {
	      m->table[index].key = copy;
	      m->table[index].value = value;
	      m->size++;
	      return true;
	    }
	  else
	    {
	      return false;
	    }
	}
    }
  else
    {
      // NULL matrix
      return false;
    }
}

void smap_embiggen(smap *m)
{
  // TO-DO: 60 write and test embiggen for hash table
}

bool smap_contains_key(const smap *m, const char *key)
{
  return smap_table_find_key(m->table, key, m->hash, m->size, m->capacity) < m->size;
}

int *smap_get(smap *m, const char *key)
{
  int index = smap_table_find_key(m->table, key, m->hash, m->size, m->capacity);
  if (index < m->size)
    {
      return m->table[index].value;
    }
  else
    {
      return NULL;
    }
}

void smap_for_each(smap *m, void (*f)(const char *, int *))
{
  for (int i = 0; i < m->size; i++)
    {
      f(m->table[i].key, m->table[i].value);
    }
}  

void smap_for_each_r(smap *m, void (*f)(const char *, int *, void *), void *arg)
{
  for (int i = 0; i < m->size; i++)
    {
      f(m->table[i].key, m->table[i].value, arg);
    }
}  

void smap_destroy(smap *m)
{
  // free all the copies of the keys we made
  for (int i = 0; i < m->size; i++)
    {
      free(m->table[i].key);
    }

  // free the table
  free(m->table);

  // free the smap struct
  free(m);
}

	       

