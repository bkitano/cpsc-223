#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "cooccur.h"
#include "smap.h"

struct cooccurrence_matrix {
    char ** keywords;
    int n; // number of keywords
    smap * table; // pointer to smap
};

int hash(const char * word);

/**
 * Creates a cooccurrence matrix that counts cooccurrences of the given keywords
 * and is initialized to 0 for all entries.
 *
 * @param key an array of distinct non-NULL strings, non-NULL
 * @param n the size of that array
 * @return a pointer to a new cooccurrence matrix; the caller is responsible for
 * destroying the matrix
 */
 
cooccurrence_matrix *cooccur_create(char *key[], int n) {
    
    // make space for the struct
    cooccurrence_matrix * c = malloc(sizeof(cooccurrence_matrix));
    
    // make space to store the keywords
    c->keywords = (char **) malloc(sizeof(char *) * n);
    
    for(int i = 0; i < n; i++) {
        
        // make space to store a single keyword
        char *copy = malloc(strlen(key[i]) + 1);
        strcpy(copy, key[i]);
        
        c->keywords[i] = copy;  
    }
    
    c->n = n;
    
    // malloc'ing here to create the smap
    c->table = smap_create(hash);
    
    // add the keywords 
    for(int i = 0; i < n; i++) {
        
        // initialize a row filled with n 0's
        int * row = malloc(sizeof(int) * n);
        for(int j = 0; j < n; j++) {
            row[j] = 0;
        }
        
        smap_put(c->table, c->keywords[i], row);
    }
    return c;
}

/**
 * Updates the given cooccurrence matrix by incrementing the counts for
 * each pair of keywords in the given context.
 *
 * @param mat a pointer to a cooccurrence matrix, non-NULL
 * @param context an array of distinct non-NULL strings that are keywords
 * for that matrix, non-NULL
 * @param n the size of that array
 */
void cooccur_update(cooccurrence_matrix *mat, char **context, int n);

/**
 * Reads keywords from the given matrix from the current line of the given stream.
 * A keyword is considered to have been read if it appears on the stream before
 * the next newline surrounded by whitespace, the beginning of input (as seen by
 * this function), or EOF.  The newline is read and removed from the stream.
 *
 * @param mat a pointer to a cooccurrence matrix, non-NULL
 * @param stream a stream, non-NULL
 * @param n a pointer to an int where the size of the returned array will be written
 * @return an array of unique non-NULL strings containing all the keywords read;
 * the caller is responsible for deallocating the array and the strings it contains
 */
char **cooccur_read_context(cooccurrence_matrix *mat, FILE *stream, int *n);

/**
 * Returns the vector (row) for the given word in the given matrix.
 * Values in the returned array correspond to the keywords for the
 * given matrix in the order they were given in the array passed to
 * the call to cooccur_create that created the given matrix.  Each
 * value is the proportion of times that the given keyword appeared in
 * a call to cooccur_update for the given matrix that also contained
 * each other keyword.  If the given word is not a keyword for the
 * given matrix or the keyword has never appeared in a call to
 * cooccur_update for the given matrix, then the returned array
 * contains 0.0 in every entry.
 *
 * @param mat a pointer to a cooccurrence matrix, non-NULL
 * @param word a string, non-NULL
 * @return an array of doubles; it is the caller's responsibility to deallocate that array
 */
double *cooccur_get_vector(cooccurrence_matrix *mat, const char *word);

/**
 * Destroys the given matrix.
 * 
 * @param mat a pointer to a cooccurrence matrix, non-NULL
 */
void cooccur_destroy(cooccurrence_matrix *mat) {
    
    for(int i = 0; i < mat->n; i++) {
        free(smap_get(mat->table, mat->keywords[i]));
        free(mat->keywords[i]);
    }
    
    // free the smap
    smap_destroy(mat->table);

    
    free(mat->keywords);
    
    free(mat);

}

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
    
    char *key[4] = {"asdf", "ghjk", "qwer", "yuio"};
    
    cooccurrence_matrix * c = cooccur_create(key, 4);
    
    cooccur_destroy(c);
    
    return 1;
}
