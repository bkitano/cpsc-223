#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "smap.h"

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

/**
 * Creates an empty map that uses the given hash function.
 *
 * @param h a pointer to takes a string and returns its hash code
 * @return a pointer to the new map or NULL if it could not be created;
 * it is the caller's responsibility to destroy the map
 */
smap *smap_create(int (*h)(const char *s)) {
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

/**
 * Returns the number of (key, value) pairs in the given map.
 *
 * @param m a map, non-NULL
 * @return the size of m
 */
int smap_size(const smap *m) {
    return m->size;
}

/**
 * Adds a copy of the given key with value to this map.
 * If the key is already present then the old value is replaced.
 * The caller retains ownership of the value.
 *
 * @param m a map, non-NULL
 * @param key a string, non-NULL
 * @param value a pointer to an integer
 * @return true if the put was successful, false otherwise
 */
bool smap_put(smap *m, const char *key, int *value) {
    // if the map is full, expand it using malloc
        // if malloc fails, return false
    
    // make the hash
    // add the value to the array
    // return true
}

/**
 * Determines if the given key is present in this map.
 *
 * @param m a map, non-NULL
 * @param key a string, non-NULL
 * @return true if key is present in this map, false otherwise
 */
bool smap_contains_key(const smap *m, const char *key);

/**
 * Returns the value associated with the given key in this map.
 * If the key is not present in this map then the returned value is
 * NULL.  The value returned is the original value passed to smap_put,
 * and it remains the responsibility of whatever called smap_put to
 * release the value (no ownership transfer results from smap_get).
 *
 * @param m a map, non-NULL
 * @param key a string, non-NULL
 * @return the assocated value, or NULL if they key is not present
 */
int *smap_get(smap *m, const char *key);

/**
 * Calls the given function for each (key, value) pair in this map.
 *
 * @param m a map, non-NULL
 * @param f a pointer to a function that takes a key and a value, non-NULL
 */
void smap_for_each(smap* m, void (*f)(const char *k, int *v));

/**
 * Calls the given function for each (key, value) pair in this map, passing
 * the extra argument as well.
 *
 * @param m a map, non-NULL
 * @param f a pointer to a function that takes a key, a value, and an
 * extra piece of information, non-NULL
 * @param arg a pointer
 */
void smap_for_each_r(smap *m, void (*f)(const char *, int *, void *), void *arg);

/**
 * Destroys the given map.
 *
 * @param m a map, non-NULL
 */
void smap_destroy(smap *m);

