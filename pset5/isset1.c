#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "isset.h"

// debug switch
#define DEBUG
#define VERBOSE
// #define PTREE

typedef struct _isset_node {
  int start;
  int end;
  struct _isset_node *parent;
  struct _isset_node *left;
  struct _isset_node *right;
  int size;
} isset_node;

typedef struct _isset_search_result {
  isset_node *parent; // the parent node
  isset_node *n; // the node (interval) where it belongs
  bool found; // true if found
  int dir; // 0 if found, negative if left of interval, positive if right
  isset_node *last_left;
  isset_node *last_right;
} isset_search_result;

struct isset {
  isset_node *root;
  int size; // total range of intervals
  int nodes; // total number of nodes in tree
};

isset_node *isset_create_node(int item);
void isset_delete_node(isset* s, isset_node *node);
void isset_find_node(const isset *s, int item, isset_search_result *result);
int isset_interval_compare(const isset *s, int start, int end, int item);
void isset_destroy_subtree(isset_node *n);
void isset_print_subtree(const isset_node *node, int level);
int getHeight(isset_node * n);
void rotateTree(isset *s, isset_node *root, int direction);
void rebalanceTree(isset *s, isset_node *root);
void totalRebalance(isset *s, isset_node *root);


// 1625 already implemented correctly
isset *isset_create() {
  isset *s = malloc(sizeof(isset));

  if (s != NULL)
    {
      s->root = NULL;
      s->size = 0;
      s->nodes = 0;
    }

  return s;
}

// implemented basically
int isset_size(const isset *s) {
  if (s != NULL)
    {
      return s->size;
    }
  else
    {
      return 0;
    }
}

/**
 * Why does this implementation make sense?
 * 
 * Consider the difference in the construction of isset. We have 'nodes' and 
 * 'size.' What's the difference?
 * 
 * size is the number of integers in the tree. Nodes is the number of intervals.
 */ 
int isset_count_intervals(const isset *s) {
  if (s != NULL) {
      return s->nodes;
  }
  else {
      return 0;
  }
}

// implicitly implemented correctly
bool isset_contains(const isset *s, int item) {
  if (s == NULL) {
      return false;
  }
  else {
      isset_search_result result;
      isset_find_node(s, item, &result);
      
      return result.found;
  }
}


/**
 * Finds the node containing the given integer in the given set's tree,
 * or, if the integer is not there, determines where it would be added.
 *
 * @param s a pointer to a set, non-NULL
 * @param item an integer
 * @param return a pointer to the struct in which to record the results
 * of the search
 * 
 * DEPENDENCIES: isset_interval_compare
 * 
 * I believe this is implemented correctly.
 */
void isset_find_node(const isset *s, int item, isset_search_result *result) {
  
  printf("find item: %d\n", item);
  
  isset_node *parent = NULL;
  isset_node *curr = s->root; // the current node you're checking
  result->last_left = NULL; 
  result->last_right = NULL;
  
  int dir = 0;
  
  while (curr != NULL && (dir = isset_interval_compare(s, curr->start, curr->end, item)) != 0) {
      parent = curr;
      if (dir < 0) { // if the item is one the left of the interval
    	  result->last_left = curr;
    	  curr = curr->left;
    	} else { // otherwise, it's on the right of the interval
    	  result->last_right = curr;
    	  curr = curr->right;
  	  }
  }

  // when you've found the interval it belongs in
  result->parent = parent;
  result->n = curr;
  result->dir = dir;
  result->found = (curr != NULL);
}


/**
 * Determines if the given integer comes before, within, or after the given
 * interval.
 *
 * @param s a pointer to a set, non-NULL
 * @param start an integer
 * @param end an integer greater than or equal to start
 * @param item an integer
 * @return a negative integer if item < start, a positive integer if
 * item > start, and 0 if start <= item <= end
 * 
 * so say an interval is -3 to 5
 * if item = 4, then left = 4-(-3) = 7, right = 4 - 5 = -1, so it's in inside
 * if item = -4, then left = -4 - (-3) = -1, right = -4 - 5 = -9, so it's to the left
 * if item = 6, then left = 6 - -3 = 9, right = 4 - 5 = 1, so it's to the right
 * 
 * if a number is in the interval, then it is greater than the start
 * and less than the end, so:
 * left = item > start ==> item - start > 0 and
 * right = item < end ==> item - end < 0
 * 
 * if a number is to the left of the interval, then
 * item < start ==> left = item - start < 0
 * thus the return value will be negative.
 * 
 * if a number is to the right of the interval, then
 * item > end ==> right = item - end > 0
 * thus the return value will be positive.
 * 
 * DEPENDENCIES: none
 * 
 * I believe this is implemented correctly.
 */
int isset_interval_compare(const isset *s, int start, int end, int item) {
  
  int left = item - start;
  int right = item - end;

  if (left >= 0 && right <= 0)
    {
      return 0;
    }
  else if (left < 0)
    {
      return left;
    }
  else
    {
      return right;
    }
}

/**
 * Adds the given integer to the given set.
 *
 * @param s a pointer to a set, non-NULL
 * @param item an integer
 * @return true if and only if the item was not in the set and was added
 * 
 * DEPENDENCIES: isset_find_node
 */
bool isset_add(isset *s, int item) {
  
  if (s == NULL) {
      return false;
  }
  
  // check if item is already in the set
  isset_search_result result;
  isset_find_node(s, item, &result);
  if (result.found) {
      // in the set already; nothing more to do
      return false;
  }
  
  /**
   * this first set of if statements checks to see if the item
   * we want to add is directly adjacent to the parent interval.
   * If it is, we can simply augment the interval of the parent 
   * node, rather than having to create a whole new node. This also
   * allows us to calculate the number of intervals immediately.
   */

  if (result.dir < 0) {
    // moved left; see if can prepend to parent
    if (result.parent->start == item + 1) {
  	  result.parent->start = item;
  	  s->size++;

  	  return true;
    }
  } else if (result.dir > 0) {
    // moved right; see if can append to parent
    if (result.parent->end == item - 1) {
  	  result.parent->end = item;
  	  s->size++;

  	  return true;
    }
  }

  /**
   * Since it wasn't directly adjacent to an existing interval, 
   * we need to place it into the tree. This is where we need
   * to be doing rebalancing. 
   * 
   * Why is it a ** and not a * ?
   * because then we'd be malloc'ing space that's already
   * allocated
   */
  isset_node **incoming;
  if (result.parent == NULL) {
      incoming = &s->root; // these values are NULL anyways
  } else if (result.dir < 0) {
    incoming = &result.parent->left;
  } else {
    incoming = &result.parent->right;
  }
  
  isset_node *new_node = isset_create_node(item);
  *incoming = new_node;

  if (new_node != NULL) {
      new_node->parent = result.parent;
      s->size++;
      s->nodes++;
      totalRebalance(s, s->root);
      
      #ifdef PTREE
      isset_print_subtree(s->root, 0);
      #endif
      
      return true;
  } else {
      return false;
  }
}


/**
 * getHeight: recursively determines the height of a tree.
 * @param n a node in a tree
 */
int getHeight(isset_node * n) {
  
  
  if(n == NULL) { // if it's a null pointer
    
    return -1;
  } else { // else, take the max of the left and right + 1
    
    int left_height = getHeight(n->left);
    int right_height = getHeight(n->right);
    return ((left_height > right_height ? left_height : right_height) + 1);
    
  }
  
}

//LEFT-RIGHT-ROTATE FUNCTIONS
void rotateTree(isset * s, isset_node * x, int dir) {
  // right rotation
  if(dir > 0) {
    
    isset_node *y;
    
    y = x->left;
    
    //rotate
    if(y != NULL) {
      x->left = y->right;
      if(y->right != NULL) {
        y->right->parent = x;
      }
      y->parent = x->parent;
    }
    
    //if x no parent, set new y to be root
    if(x->parent == NULL) {
      s->root = y;
    }
    
    //if x had parent
    else {
      if(x->parent->left != NULL && x->start == x->parent->left->start) {
        x->parent->left = y;
      } else if(x->parent->right != NULL && x->start == x->parent->right->start) {
         x->parent->right = y;
      }
    } 
    
    if(y != NULL) {
      y->right = x;
    }
    
    x->parent = y;
  } 
  // left rotation
  else {
    isset_node *y;
    y = x->right;
    
    //rotate
    if(y != NULL) {
      x->right = y->left;
      if(y->left != NULL) {
        y->left->parent = x;
      }
      y->parent = x->parent;
    } 
    
    if(x->parent == NULL) {
      s->root = y;
    }
    
  //otherwise, x was root (no parent)
    else {
      if(x->parent->left != NULL && x->start == x->parent->left->start) {
         x->parent->left = y;
      } 
      else if(x->parent->right != NULL && x->start == x->parent->right->start) {
          x->parent->right = y;
      }
    }
      
    if(y != NULL) {
      y->left = x;
    }
    x->parent = y;
  }
}


/**
 * rebalanceTree: given the grandparent node of an issue, rebalance the tree
 * @param root: a pointer to the pointer of a node
 */
void rebalanceTree(isset * s, isset_node * root) {
    
    // check which of 4 ways we need to rotate:
    // left imbalance
    if(getHeight(root->left) > getHeight(root->right) + 2) { 
      
      // check which grandchild is the problem
      if( getHeight( root->left->right ) > getHeight( root->left->left ) ) {
        // a left right problem, so a left rotation on middle, then right rotation on grandparent
        rotateTree(s, root->left, -1);
      } // otherwise it's a left left problem, so just a right rotation on grandparent
      rotateTree(s, root, 1);
    }
     
    // right imbalance 
    else if ( getHeight(root->right) >= getHeight(root->left) + 2) { // right imbalance
      if( getHeight(root->left) > getHeight(root->right)) { // a right left problem, so a right rortation on middle, then left rotation on grandparent
        rotateTree(s, root->right, 1);
      } // otherwise, a right right problem, so just a left rotation on grandparent
      rotateTree(s, root, -1);
    }
}

/**
 * totalRebalance: recursively parse the tree from the top down. If that
 * node is imbalanced, balance it; if not, balance the left and right
 * @param root: the starting node
 */
 
void totalRebalance(isset * s, isset_node * root) {
  rebalanceTree(s, root);
  
  if(root->left) {
    totalRebalance(s, root->left );
  }
  
  if(root->right) {
    totalRebalance(s, root->right );
  }
}

/**
 * Returns a pointer to a new node containing the interval [item, item].
 *
 * @param item an integer
 * @return a pointer to a new node containing the interval [item, item]
 */
isset_node *isset_create_node(int item) {
  isset_node *result = (isset_node *)malloc(sizeof(isset_node));

  if (result != NULL) {
      result->right = NULL;
      result->left = NULL;
      result->start = item;
      result->end = item;
      result->parent = NULL;
  }

  return result;
}

bool isset_remove(isset *s, int item) {
  printf("item to remove: %d\n", item);
    isset_search_result result;
    isset_find_node(s, item, &result); 

    if(result.found) {
      #ifdef VERBOSE
      printf("found");
      #endif
        isset_node * n = result.n;
        
        // is it at the end of an interval?
        if(item == n->end && item != n->start ) {
            // make the interval smaller
            n->end--;
            // update the size
            n->size--;
            return true;
        } 
        
        // is it at the beginning of an interval?
        else if(item == n->start && item != n->end ) {
            
            // make the interval smaller
            n->start++;
            // update the size
            n->size--;
            return true;
        } 
        
        // is it an entire interval?
        else if(item == n->start && item == n->end ) {
            // just delete the whole thing
            isset_delete_node(s, n);
            
            // update the size
            s->size--;
            
            // we removed a node, so decrement
            s->nodes--;
            return true;
        }
        
        // is it in the middle of an interval?
        else {
          
          // we break up the original interval into two sub intervals
          int new_start = n->start;
          
          // for the left side, we create a new node
          isset_node * new_left = isset_create_node(new_start);
          new_left->end = item - 1;
          
          if(n->left) {
            n->left->parent = new_left;
          }
          new_left->left = n->left;
          
          new_left->parent = n; // position this new left below the original
          n->left = new_left;
          
          // now, reset the original
          n->start = item + 1;
          
          s->size--;
          s->nodes++;
          return true;
        }
    } else {
      printf("not found");
        return false;
    }
}

/**
 * Deletes the given node in the given BST.
 *
 * @param s a pointer to a set, non-NULL
 * @param node a node in that set's tree
 */
void isset_delete_node(isset *s, isset_node *node) {
  
  #ifdef VERBOSE
  printf("entered isset_delete_node\n");
  #endif
  
  // find pointer to the node to delete
  isset_node **incoming;
 
 /**
  * the pointer incoming stores the parent's pointer to its
  * left or right child, whichever is the one to be deleted. 
  * Then, when we need to change that to whatever, we can do 
  * it quickly.
  */ 
  if (node->parent == NULL) { // if we are deleting the root
    #ifdef VERBOSE
    printf("deleting the root \n");
    #endif
  
    incoming = &s->root;
  } 
  
  else if (node->parent->left == node) { // if we are deleting a node on the left
    #ifdef VERBOSE
    printf(" deleting a left child\n");
    #endif
  
      incoming = &node->parent->left;
  } 
  
  else {
    
      #ifdef VERBOSE
      printf("deleting a right child \n");
      #endif
      
      incoming = &node->parent->right; // we are deleting a node on the right
  }
  /**
   * Here, because incoming is a leaf, we can safely set the pointer
   * which incoming points to to a null pointer.
   */
  if (node->left == NULL && node->right == NULL) { // if it's a leaf
  
    #ifdef VERBOSE
  printf("deleting a leaf\n");
  #endif
  
      *incoming = NULL;
  } 
  
  /**
   * Here, the node has one child. In the case of having a right child:
   * 1. store the pointer of node->right
   * 2. change the pointer of node->right->parent to node->parent
   * 3. change *incoming to node->right 
   * (set the parent's new child to node->right)
   */ 
  // one child cases
  else if (node->left == NULL) {
    
      #ifdef VERBOSE
  printf("only child on the right\n");
  #endif
    
      *incoming = node->right;
      node->right->parent = node->parent;
      
  } else if (node->right == NULL) {
    
      #ifdef VERBOSE
  printf("only child on the left\n");
  #endif
  
      *incoming = node->left;
      node->left->parent = node->parent;
  } 
  
  /**
   * In the two child case, we need to identify the successor
   * node, which is the next largest node. This node is also
   * the smallest node of the right subtree, so we will 
   * traverse the right subtree until we have found reached the 
   * left most leaf. This value will be our replacement.
   */ 
  // two children case
  else {
    
      #ifdef VERBOSE
  printf("single parent, two kids\n");
  #endif
    
    // TWO CHILDREN CASE (not needed until we do isset_remove)
    
    // 1. find the successor
    
    // a. initialize a starter, the root of the right subtree
    isset_node * successor = node->right;
    
    // b. while the starter's left child isn't null
    while(successor->left) {
    
      // c. reset successor to be the left child.
      successor = successor->left;
    }
    
    /** 2. now that we have found the successor (note that it doesn't
    * have a left child, since we recursed to the bottom!), 
    * we need to do a few things: 
    */ 
    
    #ifdef VERBOSE
    printf("successor: [%d %d]\n", successor->start, successor->end);
    #endif
    
    // replace successor->parent->left to successor->right
    // remember that successor doesn't have a left child, so this is safe
    if(successor->right) {
      successor->parent->left = successor->right;
      successor->right->parent = successor->parent;
    }
    
    // set all of the subtrees of node to be the subtrees of successor
    successor->left = node->left;
    if(node->left) {
      node->left->parent = successor;
    }
    
    if(node->right != successor) {
      successor->right = node->right;
      node->right->parent = successor;
    }
    
    // finally, set incoming to successor.
    
    *incoming = successor;
  }
  
  // free space for node
  free(node);
  s->nodes--;
  
}

int isset_next_excluded(const isset *s, int item)
{
  while (isset_contains(s, item))
    {
      item++;
    }
  return item;
}

// working
void isset_destroy(isset *s)
{
  isset_destroy_subtree(s->root);
  s->size = 0;
  s->nodes = 0;
  s->root = NULL;
  free(s);
}


/**
 * Deletes all of the nodes in the tree rooted at the given node.
 * There is no effect if given pointer is NULL.
 *
 * @param n a pointer to a node
 */
void isset_destroy_subtree(isset_node *n)
{
  if (n != NULL)
    {
      isset_destroy_subtree(n->left);
      isset_destroy_subtree(n->right);
      free(n);
    }
}

/**
 * Prints the contents of the subtree rooted at the given node,
 * or "null" if the given pointer is NULL.
 *
 * @param node a pointer to a node
 * @param level the depth of that node
 */
void isset_print_subtree(const isset_node *node, int level)
{
  for (int i = 0; i < level; i++)
    {
      printf(" . ");
    }
  if (node != NULL)
    {
      printf("[%d-%d]\n", node->start, node->end);
      isset_print_subtree(node->left, level + 1);
      isset_print_subtree(node->right, level + 1);
    }
  else
    {
      printf("null\n");
    }
}


#ifdef DEBUG
int main(int argc, char **argv) {
  printf("debugging:\n");
  
  isset * t = isset_create();
  isset_add(t, 5);
  isset_add(t, 6);
  isset_add(t, 7);
  
  printf("tree size: %d\n", t->size);
  printf("tree nodes: %d\n", t->nodes);
  printf("tree root pointer: %p\n", (void *) (t->root));
  
  int height = getHeight( t->root );
  printf("height: %d\n", height);
  
  isset_print_subtree( t->root, 0);
  
  // delete testing
  isset_remove(t, 6);
  
  isset_print_subtree( t->root, 0);
  
  printf("asdfasdfasdf");
  isset_destroy(t);
}
#endif
