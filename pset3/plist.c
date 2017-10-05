#include <stdio.h>
#include <stdlib.h>

#include "point.h"
#include "plist.h"

#define LIST_INITIAL_SIZE 2

/**
 * Creates an empty list of points.
 * 
 * @return a pointer to the list, or NULL if allocation was unsuccessful
 */
plist *plist_create() {
    plist * clist = (plist *) malloc(sizeof(plist));
    clist->distance = 0;
    clist->size = 0;
    clist->points = malloc(sizeof(point) * LIST_INITIAL_SIZE);
    clist->capacity = LIST_INITIAL_SIZE;
    
    return clist;
}

/**
 * Destroys the given list.  The space occupied by the list and all
 * its elements is released.
 *
 * @param l a pointer to a list, non-NULL
 */
void plist_destroy(plist *l) {
    free(l->points);
    free(l);
}

/**
 * Returns the number of points on the given list.
 *
 * @param l a pointer to a list, non-NULL
 * @return the number of points in l
 */
int plist_size(const plist *l) {
    return(l->size);
}

/**
 * Add a copy of the given point to the end of the given list.
 *
 * @param l a pointer to a list, non-NULL
 * @param p a pointer to a point, non-NULL
 * @return true if and only if the point was successfully added
 */
bool plist_add_end(plist *l, const point *p) {

    // if the list is at capacity
    if (l->size == l->capacity) {
        
        // make plist bigger
        int largerCap = (l->capacity * 2 > LIST_INITIAL_SIZE ? l->capacity * 2 : LIST_INITIAL_SIZE);
        point *larger = realloc(l->points, largerCap * sizeof(point));
        
        if (larger != NULL) {
            l->points = larger;
            l->capacity = largerCap;
        } else {
            return false;
        }
    }
        
    else if (l->size < l->capacity) {
        l->points[l->size] = *p;
        l->size++;
        return true;
    }
    return false;
}

/**
 * Copies the point at the given location in the given list into the
 * given point.  There is no effect if the index is not valid.
 *
 * @param l a pointer to a list, non-NULL
 * @param i an integer
 * @param p a pointer to a point, non-NULL
 */
void plist_get(const plist *l, int i, point *p) {
    if(i < l->size) {
        point * points = l->points;
        p = &points[i];
    }
}

/**
 * Determines if the given point is contained in the given list.
 *
 * @param l a pointer to a list, non-NULL
 * @param p a pointer to a point, non-NULL
 * @return true if and only if l contains a point with the same coordinates as p
 */
bool plist_contains(const plist *l, const point *p) {
    // for loop
    point * points = l->points;
    for(int i = 0; i < l->size; i++) {
        point test = points[i];
        if(test.x == p->x) {
            if(test.y == p->y) {
                return true;
            }
        }
    }
    return false;
}

/**
 * Prints the given list to the given stream using the given format
 * for each point.  If there is a newline at the end of the format
 * specifier then that newline is printed once at the end of the list.
 * The list will be enclosed in square brackets and each point in the
 * list will be output in the format printed by
 * <code>point_fprintf</code> when passed the format specifier with
 * the trailing newline removed.  Points will be separated by a comma
 * and a single space.
 *
 * @param stream a pointer to astream, non-NULL
 * @param fmt a format specifier string, non-NULL
 * @param l a pointer to a list, non-NULL
 */
void plist_fprintf(FILE *stream, const char *fmt, const plist *l);

/**
 * Sorts the given list using the given comparator.
 *
 * @param l a pointer to a list, non-NULL
 * @param compare a pointer to a function that returns a negative
 * number to indicate the first point comes before the second, a
 * positive number to indicate the second point comes before the
 * first, and zero if the have the same ordinal value.
 */
void plist_sort(plist *l, int (*compare)(const point*, const point*));
