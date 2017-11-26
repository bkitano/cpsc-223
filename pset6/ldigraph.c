#define _GNU_SOURCE

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "ldigraph.h"


struct ldigraph {
  int n;          // the number of vertices
  int *list_size; // the size of each adjacency list
  int *list_cap;  // the capacity of each adjacency list
  int **adj;      // the adjacency lists
};

typedef struct node {
  int index;
  int outdegree;
} node;

struct ldig_search {
  const ldigraph *g;
  int from;
  int *color;
  int *dist;
  int *pred;
};

// used to mark what vertices are unexplored (white), 
enum {WHITE, GRAY, BLACK};

#define LDIGRAPH_ADJ_LIST_INITIAL_CAPACITY 4

/**
 * Resizes the adjacency list for the given vertex in the given graph.
 * 
 * @param g a pointer to a directed graph
 * @param from the index of a vertex in that graph
 */
void ldigraph_list_embiggen(ldigraph *g, int from);

/**
 * Visits the given vertex in the given search of the given graph.
 *
 * @param g a pointer to a directed graph
 * @param s a search in that graph
 * @param from a vertex in that graph
 */
void ldigraph_dfs_visit(const ldigraph* g, ldig_search *s, int from);

/**
 * Prepares a search result for the given graph starting from the given
 * vertex.  It is the responsibility of the caller to destroy the result.
 *
 * @param g a pointer to a directed graph
 * @param from the index of a vertex in that graph
 * @return a pointer to a search result
 */
ldig_search *ldig_search_create(const ldigraph *g, int from);

/**
 * Visits the given vertex in the given search of the given graph, 
 * but sorts according to the spec before iterating.
 * 
 * @param g a pointer to a directed graph
 * @param s the search result
 * @param from the start
 * @param comparator the compare function for qsort
 */ 
void ldigraph_ofs_visit(const ldigraph* g, ldig_search *s, int from, int (*comparator)(const node*, const node*));

/**
 * Does the comparison specified by the spec.
 */
int comparator(const node * a, const node * b);


ldigraph *ldigraph_create(int n) {
  if (n < 1) {
    return NULL;
  }
  
  ldigraph *g = malloc(sizeof(ldigraph));
  if (g != NULL) {
    g->n = n;
    g->list_size = malloc(sizeof(int) * n);
    g->list_cap = malloc(sizeof(int) * n);
    g->adj = malloc(sizeof(int *) * n);
  
    if (g->list_size == NULL || g->list_cap == NULL || g->adj == NULL) {
      free(g->list_size);
      free(g->list_cap);
      free(g->adj);
      free(g);
      
      return NULL;
    }
  
    for (int i = 0; i < n; i++) {
      g->list_size[i] = 0;
      g->adj[i] = malloc(sizeof(int) * LDIGRAPH_ADJ_LIST_INITIAL_CAPACITY);
      g->list_cap[i] = g->adj[i] != NULL ? LDIGRAPH_ADJ_LIST_INITIAL_CAPACITY : 0;
    }
  }
  
  return g;
}

int ldigraph_size(const ldigraph *g) {
  if (g != NULL) {
    return g->n;
  }
  else {
    return 0;
  }
}

void ldigraph_list_embiggen(ldigraph *g, int from) {
  if (g->list_cap[from] != 0) {
    g->adj[from] = realloc(g->adj[from], sizeof(int*) * g->list_cap[from] * 2);
    g->list_cap[from] = g->adj[from] != NULL ? g->list_cap[from] * 2 : 0;
  }
}

void ldigraph_add_edge(ldigraph *g, int from, int to) {
  if (g != NULL && from >= 0 && to >= 0 && from < g->n && to < g->n && from != to) {
  // make room if necessary
    if (g->list_size[from] == g->list_cap[from]) {
      ldigraph_list_embiggen(g, from);
    }
  
  // add to end of array if there is room
    if (g->list_size[from] < g->list_cap[from]) {
      g->adj[from][g->list_size[from]++] = to;
    }
  }
}

bool ldigraph_has_edge(const ldigraph *g, int from, int to) {
  if (g != NULL && from >= 0 && to >= 0 && from < g->n && to < g->n && from != to) {
// sequential search of from's adjacency list
    int i = 0;
    while (i < g->list_size[from] && g->adj[from][i] != to) {
      i++;
    }
    return i < g->list_size[from];
  }
  else {
    return false;
  }
}

// #define bfsdbg
ldig_search *ldigraph_bfs(const ldigraph *g, int from) {

  ldig_search * s = ldig_search_create(g, from); // the search result

  int * q = malloc(sizeof(int) * g->n); // a queue to track the layers

  if(q != NULL) {
    // initialize all the values
    for(int i = 0; i < g->n; i++) {
      q[i] = -1;
    }
  
    // dummies to track the queue
    int head = 0;
    int tail = 0;
    
    // add the root to the queue
    q[tail] = from;
    s->dist[from] = 0;
    tail++;
    
    #ifdef bfsdbg
      for(int i = 0; i < g->n; i++) {
        printf("%d\n", q[i]);
      }
      printf("%d\n", head);
      printf("%d\n", q[head]);
      
    #endif
    
    int curr = q[head];
    while(head < tail) {
      
      // the current node that we just pulled from the top of the queue
      curr = q[head];
      
      // set its color to black, since we are now visiting
      s->color[curr] = BLACK;
      
      // for each neighbor:
      for(int i = 0; i < g->list_size[curr]; i++) {
        int neighbor = g->adj[curr][i];
        
        // if they are white, aka not yet visited:
        if(s->color[neighbor] == WHITE) {
          // 1. enqueue them
          q[tail] = neighbor;
          tail++;
          
          // 2. set their color to gray, as they are now queued up to visit
          s->color[neighbor] = GRAY;
          
          // 3. set their pred to the curr
          s->pred[neighbor] = curr;
          
          // 4. set their distance to be 1+curr
          s->dist[neighbor] = s->dist[curr] + 1;
        } // else, do nothing
      }
      // dequeue by moving the head over
      head++;
    } // end of while
    
    free(q);
  }
  return s;
}

ldig_search *ldigraph_dfs(const ldigraph *g, int from) {
  if (g == NULL || from < 0 || from >= g->n) {
    return NULL;
  }

  ldig_search *s = ldig_search_create(g, from);
  if (s != NULL) {
    // start at from
    s->color[from] = GRAY;
    s->dist[from] = 0;
    ldigraph_dfs_visit(g, s, from);
  }
  return s;
}

ldig_search *ldigraph_ofs(const ldigraph *g, int from) {
  if (g == NULL || from < 0 || from >= g->n) {
    return NULL;
  }

  ldig_search *s = ldig_search_create(g, from);
  if (s != NULL) {
    // start at from
    s->color[from] = GRAY;
    s->dist[from] = 0;
    ldigraph_ofs_visit(g, s, from, comparator);
  }
  return s;
}

void ldigraph_dfs_visit(const ldigraph* g, ldig_search *s, int from) {
  // get vertices we can reach in one edge from from
  const int *neighbors = g->adj[from];
  
  // iterate over outgoing edges
  for (int i = 0; i < g->list_size[from]; i++) {
    int to = neighbors[i];
    if (s->color[to] == WHITE) {
	  // found an edge to a new vertex -- explore it
  	  s->color[to] = GRAY;
  	  s->dist[to] = s->dist[from] + 1;
  	  s->pred[to] = from;
  	  
  	  // probably will 
  	  ldigraph_dfs_visit(g, s, to);
	  }
  }
  
  // mark current vertex finished
  s->color[from] = BLACK;
}

#define ofsdbg
void ldigraph_ofs_visit(const ldigraph* g, ldig_search *s, int from, int (*comparator)(const node*, const node*)) {
  // get vertices we can reach in one edge from from
  const int *neighbors = g->adj[from];
  
  // create an array of nodes from lowest to highest degree
  node * nodes = malloc(sizeof(node) * g->list_size[from]);
  assert(nodes);
  
  // fill the array
  for(int i = 0; i < g->list_size[from]; i++) {
    nodes[i].index = neighbors[i];
    nodes[i].outdegree = g->list_size[neighbors[i]];
  }
  
  #ifdef ofsdbg
  for(int i = 0; i < g->list_size[from]; i++) {
    printf("index: %d; outdegree: %d\n", nodes[i].index, nodes[i].outdegree);
  }
  #endif
  
  // the array is unsorted; use qsort
  qsort( nodes, g->list_size[from], sizeof(node), (int(*) (const void*, const void*)) comparator );
  
  #ifdef ofsdbg
  for(int i = 0; i < g->list_size[from]; i++) {
    printf("index: %d; outdegree: %d\n", nodes[i].index, nodes[i].outdegree);
  }
  #endif
  
  // iterate over outgoing edges
  for (int i = 0; i < g->list_size[from]; i++) {
    int to = nodes[i].index;
    if (s->color[to] == WHITE) {
	  // found an edge to a new vertex -- explore it
  	  s->color[to] = GRAY;
  	  s->dist[to] = s->dist[from] + 1;
  	  s->pred[to] = from;
  	  
  	  // do this in the order of highest to lowest degree
  	  ldigraph_dfs_visit(g, s, to);
	  }
  }
  
  // mark current vertex finished
  s->color[from] = BLACK;
  
  free(nodes);
}

/**
 * Returns the larger outdegree; or, if the outdegree is
 * equal, the larger index.
 */
int comparator(const node * a, const node * b) {
  
  // get the values we need at those indices
  int a_out = a->outdegree;
  int b_out = b->outdegree;
  if( a_out == b_out ) {
    return (a->index - b->index);
  } else {
    return -(a_out - b_out);
  }
}

void ldigraph_destroy(ldigraph *g) {
  if (g != NULL) {
    for (int i = 0; i < g->n; i++) {
	    free(g->adj[i]);
	  }
    free(g->adj);
    free(g->list_cap);
    free(g->list_size);
    free(g);
  }
}

ldig_search *ldig_search_create(const ldigraph *g, int from) {
  if (g != NULL && from >= 0 && from < g->n) {
    ldig_search *s = malloc(sizeof(ldig_search));
  
    if (s != NULL) {
      s->g = g;
      s->from = from;
      s->color = malloc(sizeof(int) * g->n);
      s->dist = malloc(sizeof(int) * g->n);
      s->pred = malloc(sizeof(int) * g->n);
    
      if (s->color != NULL && s->dist != NULL && s->pred != NULL) {
      // initialize all vertices to unseen
        for (int i = 0; i < g->n; i++) {
          s->color[i] = WHITE;
          s->dist[i] = -1;
          s->pred[i] = -1;
        }
      }
      else {
        free(s->pred);
        free(s->dist);
        free(s->color);
        free(s);
        return NULL;
      }
    }
  
    return s;
  } else {
    return NULL;
  }
}

int * ldig_search_path(const ldig_search *s, int to, int *len) {
  // a path array to store the path
  // why can we set the size to be of size n? because a tree has n-1 edges.
  int n = s->g->n;
  int * path = malloc(sizeof(int) * n);
  // initialize all the values
  for(int i = 0; i < n; i++) {
    path[i] = -1;
  }
  
  int from = s->from; // the start
  int curr = to; // the end
  int step = 0; // a dummy for the length
  
  do {
    path[step] = curr;
    step++;
  } while ( step <= n && (curr = s->pred[curr]) != from);
  path[step] = curr;
  
  // if the path doesn't exist
  if(curr != from) {
    *len = -1;
    return NULL;
  } else {
  
    *len = step;
    
    // since the algorithm places things in from back to front, this will flip.
    for(int i = 0; i < step; i++) {
      int temp = path[step - i];
      path[step - i] = path[i];
      path[i] = temp;
    }
    
    return path;
  }
}

void ldig_search_destroy(ldig_search *s) {
  if (s != NULL) {
    free(s->color);
    free(s->dist);
    free(s->pred);
    free(s);
  }
}

// #define MAIN
#ifdef MAIN
int main(int argc, char **argv) {
  
  ldigraph *g = ldigraph_create(7);
  assert(g);
  
    ldigraph_add_edge(g, 0, 1);
    ldigraph_add_edge(g, 0, 3);
    ldigraph_add_edge(g, 1, 3);
    ldigraph_add_edge(g, 1, 2);
    ldigraph_add_edge(g, 2, 0);
    ldigraph_add_edge(g, 2, 3);
    ldigraph_add_edge(g, 3, 1);
    ldigraph_add_edge(g, 3, 4);
    ldigraph_add_edge(g, 3, 5);
    ldigraph_add_edge(g, 3, 6);
    ldigraph_add_edge(g, 3, 2);
    ldigraph_add_edge(g, 4, 1);
    ldigraph_add_edge(g, 4, 0);
    ldigraph_add_edge(g, 4, 5);
    ldigraph_add_edge(g, 4, 6);
    ldigraph_add_edge(g, 4, 2);
    ldigraph_add_edge(g, 4, 3);
    ldigraph_add_edge(g, 5, 0);
    ldigraph_add_edge(g, 5, 4);
    ldigraph_add_edge(g, 5, 3);
    ldigraph_add_edge(g, 5, 2);
    ldigraph_add_edge(g, 5, 1);
    ldigraph_add_edge(g, 6, 6);
    ldigraph_add_edge(g, 6, 1);
    ldigraph_add_edge(g, 6, 4);
    ldigraph_add_edge(g, 6, 3);
    ldigraph_add_edge(g, 6, 2);
    ldigraph_add_edge(g, 6, 0);
    ldigraph_add_edge(g, 6, 5);
  
  int from = 5;

  // create an array of nodes from lowest to highest degree
  node * nodes = malloc(sizeof(node) * g->list_size[from]);
  assert(nodes);
  
  int * neighbors = g->adj[from];
  
  // fill the array
  for(int i = 0; i < g->list_size[from]; i++) {
    nodes[i].index = neighbors[i];
    nodes[i].outdegree = g->list_size[neighbors[i]];
  }
  
  for(int i = 0; i < g->list_size[from]; i++) {
      printf("index: %d; outdegree: %d\n", nodes[i].index, nodes[i].outdegree);
  }
  
  // the array is unsorted; use qsort
  qsort( nodes, g->list_size[from], sizeof(node), (int(*) (const void*, const void*)) comparator );
  printf("\n");
  for(int i = 0; i < g->list_size[from]; i++) {
      printf("index: %d; outdegree: %d\n", nodes[i].index, nodes[i].outdegree);
  }
  
  ldigraph_destroy(g);
  free(nodes);
  
  return 1;
}
#endif