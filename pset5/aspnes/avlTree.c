#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "avlTree.h"

/* implementation of an AVL tree with explicit heights */

struct avlNode {
    struct avlNode *child[2];    /* left and right */
    int key;
    int height;
};

/* free a tree */
void 
avlDestroy(AvlTree t)
{
    if(t != AVL_EMPTY) {
        avlDestroy(t->child[0]);
        avlDestroy(t->child[1]);
        free(t);
    }
}

/* return height of an AVL tree */
int
avlGetHeight(AvlTree t)
{
    if(t != AVL_EMPTY) {
        return t->height;
    } else {
        return -1;
    }
}

/* return nonzero if key is present in tree */
int
avlSearch(AvlTree t, int key)
{
    if(t == AVL_EMPTY) {
        return 0;
    } else if(t->key == key) {
        return 1;
    } else {
        return avlSearch(t->child[key > t->key], key);
    }
}

#define Max(x,y) ((x)>(y) ? (x) : (y))

/* assert height fields are correct throughout tree */
void
avlSanityCheck(AvlTree root)
{
    int i;

    if(root != AVL_EMPTY) {
        for(i = 0; i < 2; i++) {
            avlSanityCheck(root->child[i]);
        }

        assert(root->height == 1 + Max(avlGetHeight(root->child[0]), avlGetHeight(root->child[1])));
    }
}

/* recompute height of a node */
static void
avlFixHeight(AvlTree t)
{
    assert(t != AVL_EMPTY);

    t->height = 1 + Max(avlGetHeight(t->child[0]), avlGetHeight(t->child[1]));
}

/* rotate child[d] to root */
/* assumes child[d] exists */
/** Picture:
 *
 *     y            x
 *    / \   <==>   / \
 *   x   C        A   y
 *  / \              / \
 * A   B            B   C
 *
 * First, we undraw the line from x to B, and draw in the line from y to B.
 * We can do this because all of the things on the left are less than
 * the parent, so it doesn't matter what happens to the lines so long as
 * they stay on the same side.
 * 
 */
static void
avlRotate(AvlTree *root, int d)
{
    AvlTree oldRoot;
    AvlTree newRoot;
    AvlTree oldMiddle;

    oldRoot = *root;
    newRoot = oldRoot->child[d];
    oldMiddle = newRoot->child[!d];

    oldRoot->child[d] = oldMiddle;
    newRoot->child[!d] = oldRoot;
    *root = newRoot;

    /* update heights */
    avlFixHeight((*root)->child[!d]);   /* old root */
    avlFixHeight(*root);                /* new root */
}


/* rebalance at node if necessary */
/* also fixes height */
static void
avlRebalance(AvlTree *t)
{
    int d;

    if(*t != AVL_EMPTY) {
        for(d = 0; d < 2; d++) {
            /* maybe child[d] is now too tall */
            if(avlGetHeight((*t)->child[d]) > avlGetHeight((*t)->child[!d]) + 1) {
                /* imbalanced! */
                /* how to fix it? */
                /* need to look for taller grandchild of child[d] */
                if(avlGetHeight((*t)->child[d]->child[d]) > avlGetHeight((*t)->child[d]->child[!d])) {
                    /* same direction grandchild wins, do single rotation */
                    avlRotate(t, d);
                } else {
                    /* opposite direction grandchild moves up, do double rotation */
                    avlRotate(&(*t)->child[d], !d);
                    avlRotate(t, d);
                }

                return;   /* avlRotate called avlFixHeight */
            }
        }
                  
        /* update height */
        avlFixHeight(*t);
    }
}

/* insert into tree */
/* this may replace root, which is why we pass
 * in a AvlTree * */
void
avlInsert(AvlTree *t, int key)
{
    /* insertion procedure */
    if(*t == AVL_EMPTY) {
        /* new t */
        *t = malloc(sizeof(struct avlNode));
        assert(*t);

        (*t)->child[0] = AVL_EMPTY;
        (*t)->child[1] = AVL_EMPTY;

        (*t)->key = key;

        (*t)->height = 0;

        /* done */
        return;
    } else if(key == (*t)->key) {
        /* nothing to do */
        return;
    } else {
        /* do the insert in subtree */
        avlInsert(&(*t)->child[key > (*t)->key], key);

        avlRebalance(t);

        return;
    }
}


/* print all elements of the tree in order */
void
avlPrintKeys(AvlTree t)
{
    if(t != AVL_EMPTY) {
        avlPrintKeys(t->child[0]);
        printf("%d\n", t->key);
        avlPrintKeys(t->child[1]);
    }
}


/* delete and return minimum value in a tree */
int
avlDeleteMin(AvlTree *t)
{
    AvlTree oldroot;
    int minValue;

    assert(t != AVL_EMPTY);

    if((*t)->child[0] == AVL_EMPTY) {
        /* root is min value */
        oldroot = *t;
        minValue = oldroot->key;
        *t = oldroot->child[1];
        free(oldroot);
    } else {
        /* min value is in left subtree */
        minValue = avlDeleteMin(&(*t)->child[0]);
    }

    avlRebalance(t);
    return minValue;
}

/* delete the given value */
void
avlDelete(AvlTree *t, int key)
{
    AvlTree oldroot;

    if(*t != AVL_EMPTY) {
        return;
    } else if((*t)->key == key) {
        /* do we have a right child? */
        if((*t)->child[1] != AVL_EMPTY) {
            /* give root min value in right subtree */
            (*t)->key = avlDeleteMin(&(*t)->child[1]);
        } else {
            /* splice out root */
            oldroot = (*t);
            *t = (*t)->child[0];
            free(oldroot);
        }
    } else {
        avlDelete(&(*t)->child[key > (*t)->key], key);
    }

    /* rebalance */
    avlRebalance(t);
}
