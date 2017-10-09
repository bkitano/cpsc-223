#define __CLOSEST_PAIR__
#ifndef __CLOSEST_PAIR__

#include "point.h"
#include "plist.h"

/**
 * Compares the two given points based on x-coordinate.
 *
 * @param p1 a pointer to a point, non-NULL
 * @param p2 a pointer to a point, non-NULL
 * @return a negative number if p1 comes before p2, positive if p1 comes
 * before p2, and 0 if they are the same
 */
int point_compare_x(const point *p1, const point *p2);

/**
 * Compares the two given points based on y-coordinate.
 *
 * @param p1 a pointer to a point, non-NULL
 * @param p2 a pointer to a point, non-NULL
 * @return a negative number if p1 comes before p2, positive if p1 comes
 * before p2, and 0 if they are the same
 */
int point_compare_y(const point *p1, const point *p2);

/**
 * Reads point from the given stream and adds them to the given list.
 *
 * @param stream a pointer to an input stream
 * @param l a pointer to a list
 * @param n a non-negative integer
 */
void read_points(FILE *stream, plist *l, int n);
/**
 * Copies the points from the source list to the destination list
 * in the order they appear in the destination list.
 *
 * @param dest a pointer to a list, non-NULL
 * @param source a pointer to a list, non-NULL
 */
void copy_list(plist *dest, const plist* source);
/**
 * Returns the closest pair of points among those on the given list.
 * The closest pair are returned in p1 and p2 and their distance is
 * returned in d.  This implementation uses the O(n log n) divide-and-conquer
 * algorithm.
 *
 * @param l a pointer to a list of at least 2 points
 * @param p1 a pointer to a point, non-NULL
 * @param p2 a pointer to a point, non-NULL
 * @param d a pointer to a double, non-NULL
 */
void closest_pair(const plist *list_x, const plist *list_y, point *p1, point *p2, double *d);
/**
 * Returns the closest pair of points among those on the given list.
 * The closest pair are returned in p1 and p2 and their distance is
 * returned in d.  This implementation uses the Theta(n^2) brute force
 * algorithm.
 *
 * @param l a pointer to a list of at least 2 points
 * @param p1 a pointer to a point, non-NULL
 * @param p2 a pointer to a point, non-NULL
 * @param d a pointer to a double, non-NULL
 */
 
void closest_pair_brute_force(const plist *l, point *p1, point *p2, double *d);
/**
 * Splits the given list by adding the leftmost half of the points
 * in order of increasing x-coordinate to the end of left and the
 * rightmost half of the points in order of increasing x-coordinate
 * to to the end of right.
 *
 * @param l a pointer to a list sorted by x-coordinate, non-NULL
 * @param left a pointer to a list, non-NULL
 * @param right a pointer to a list, non-NULL
 */
void split_list_x(const plist *l, plist *left, plist *right);
/**
 * Splits the given list by adding the leftmost half of the points
 * in order of increasing y-coordinate to the end of y_left_y and the
 * rightmost half of the points in order of increasing y-coordinate
 * to to the end of y_right.
 *
 * @param l a pointer to a list sorted by y-coordinate, non-NULL
 * @param x_left a pointer to a list containing the leftmost half of the points
 * on l in order of increasing x-coordinate
 * @param x_right a pointer to a list containing the rightmost half of the
 * points on l in order of increasing x-coordinate
 * @param y_left a pointer to a list
 * @param y_right a pointer to a list
 * 
 * @param end a pointer to a list, non-NULL
 */
void split_list_y(const plist *l, const plist *x_left, const plist *x_right,
		  plist *y_left, plist *y_right);
/**
 * Adds the points in the first given list with x-coordinates in the given
 * range to the second list in order of increasing y-coordinate
 *
 * @param list_y a pointer to a list of points sorted by y-coordinate, non-NULL
 * @param middle a pointer to a list, non-NULL
 * @param left a real number
 * @param right a real number greater than or equal to left
 */
void make_middle(const plist *list_y, plist *middle, double left, double right);
/**
 * Searches the given list for a pair of points closer than d units.
 * If such a pair is found, p1 and p2 are updated to the closest such pair
 * and d is updated to their distance.
 *
 * @param middle a pointer to a list of points sorted by y-coordinate, non-NULL
 * @param p1 a pointer to a point, non-NULL
 * @param p2 a pointer to a point, non-NULL
 * @param d a pointer to a positive real number, non-NULL
 */
void search_middle(const plist *middle, point *p1, point *p2, double *d); 

#endif