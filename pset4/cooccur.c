#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "cooccur.h"
#include "smap.h"

// space complexity: O(n^2): n + 1 + n^2 + n^2
struct cooccurrence_matrix {
    char ** keywords;
    int n; // number of keywords
    smap * table; // pointer to a dictionary mapping words to contexts
    smap * keyword_indexer; // dictionary mapping words to their index in the ordered array
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

// O(n^2)
cooccurrence_matrix *cooccur_create(char *key[], int n) {

    // make space for the struct O(1)
    cooccurrence_matrix * c = malloc(sizeof(cooccurrence_matrix));

    // make space to store the keywords O(1)
    c->keywords = (char **) malloc(sizeof(char *) * n);

    // make the keyword_indexer dictionary O(1)
    c->keyword_indexer = smap_create(hash);
    
    // O(n) total
    c->n = 0;
    for(int i = 0; i < n; i++) {
        
        // check for duplicate keys
        if(!smap_contains_key(c->keyword_indexer, key[i])) {
            
            // make a copy to store a the keyword
            char *copy = malloc(strlen(key[i]) + 1); //O(1)
            strcpy(copy, key[i]); // O(1)
            
            // create an int to store the index
            int * keyword_index = malloc(sizeof(int)); //O(1)
            *keyword_index = c->n;
            (c->n)++;

            c->keywords[*keyword_index] = copy;  // O(1)
            smap_put(c->keyword_indexer, copy, keyword_index); // O(1)
        }
    }
    
    // malloc'ing here to create the smap
    c->table = smap_create(hash); // O(1)
    // add the keywords O(n^2)
    for(int i = 0; i < c->n; i++) {
        
        // initialize a row filled with n 0's
        int * row = malloc(sizeof(int) * c->n);
        // O(n)
        if(row != NULL) {
            for(int j = 0; j < c->n; j++) {
                row[j] = 0;
            }
        }
        smap_put(c->table, c->keywords[i], row); // O(1)
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

// O(n^2)
void cooccur_update(cooccurrence_matrix *mat, char **context, int n) {
    
    // O(n^2)
    for(int i = 0; i < n; i++) {
        
        // get the current word and its vector
        char * current_word = context[i];
        int * current_row = smap_get(mat->table, current_word); // O(1)
        
        // O(n)
        for(int j = 0; j < n; j++) {
            // get the index we should increment
            int word_index = *smap_get(mat->keyword_indexer, context[j]);  // O(1)
            current_row[word_index] += 1;
        }
        
    }
}

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
 
// 
char **cooccur_read_context(cooccurrence_matrix *mat, FILE *stream, int *n) {
    
    // create an array to store the contexts
    char **keywords_appeared = malloc( (sizeof(char *)) * mat->n); // O(1)
        if(keywords_appeared != NULL) {
        // create a smap to ensure there are no duplicates
            smap * local = smap_create(hash);
            if(local != NULL) {
                // get the first line from the stream
                int max_buffer = 1000;
                char * line = malloc(sizeof(char) * max_buffer + 1);
                
                if(line != NULL) {
                    if( fgets(line, max_buffer, stream) == NULL ) {
                        smap_destroy(local);
                        free(line);
                        free(keywords_appeared);
                        return NULL;
                    }
                    strtok(line, "\n"); // strip the new line character
                
                    // a token
                    char * token;
                    
                    // get the first token
                    token = strtok(line, " ");
                    
                    // set the size to 0
                    *n = 0;
                
                    while(token != NULL) { // while it's not a null pointer
                    
                        // if it's a keyword and it's not already in the array
                        if(smap_contains_key(mat->table, token) && !smap_contains_key(local, token)) {
                            
                            char * copy = malloc(strlen(token) + 1);
                            if(copy != NULL) {
                                // make a safe copy
                                strcpy(copy, token);
                    
                                // make a dummy val to store as the value in the smap
                                int * dummy = malloc(sizeof(int));
                                *dummy = 1;
                                
                                // insert it into the local smap
                                smap_put(local, token, dummy);
                            
                                // put it in the array
                                keywords_appeared[*n] = copy;
                                // increment the size
                                (*n)++;
                            } // end of if copy
                        } // end of smap checks
                        token = strtok(NULL, " "); // move on to the next token
                    } // end of while token
                
                    free(line);
                } // end of null line    
            
                for(int i = 0; i < *n; i++) {
                    free(smap_get(local, keywords_appeared[i])); // free the dummies
                } // end of for
                
                smap_destroy(local);
            } // end of null local
        }
    return keywords_appeared;
}

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
double *cooccur_get_vector(cooccurrence_matrix *mat, const char *word) {
    // get the row
    int * row = smap_get(mat->table, word);
    
    // cast all the entries as doubles
    double * drow = malloc(sizeof(double) * mat->n);
    
    int diagonal = row[*smap_get(mat->keyword_indexer, word)];
    
    if(diagonal == 0) {
        for(int i = 0; i < mat->n; i++) {
           drow[i] = (double) row[i];
        }   
    } else {
        for(int i = 0; i < mat->n; i++) {
            drow[i] = ((double) row[i]) / ( (double) diagonal);
        }
    }
    
    return drow;
}

/**
 * Destroys the given matrix.
 * 
 * @param mat a pointer to a cooccurrence matrix, non-NULL
 */
void cooccur_destroy(cooccurrence_matrix *mat) {
    
    for(int i = 0; i < mat->n; i++) {
        
        // free the keyword_indexer pointers
        free(smap_get(mat->keyword_indexer, mat->keywords[i]));
        
        // free the table value pointer
        free(smap_get(mat->table, mat->keywords[i]));
        
        // free the keyword pointer
        free(mat->keywords[i]);
        
        
    }
    
    // free the table smap pointer
    smap_destroy(mat->table);
    
    // free the keyword_indexer smap pointer
    smap_destroy(mat->keyword_indexer);

    // free the keyword array pointer
    free(mat->keywords);
    
    // free the struct pointer
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
    
    char **keywords = malloc(sizeof(char *) * (argc-1));
    int n = 0;
    
    if(keywords != NULL) {
        
        for(int i = 1; i < argc; i++) {
            
            char * copy = malloc(strlen(argv[i]) + 1);
            if(copy != NULL) {
                strcpy(copy, argv[i]);
                keywords[i-1] = copy;
                n++;
            }
        }
        
        // create a cooccurrence_matrix
        cooccurrence_matrix * c = cooccur_create(keywords, n);
        
        /*
        // check if it instantiated properly
        for(int i = 0; i < c->n; i ++) {
            
            // check to make sure indexing and ordered access works WORKS
            printf("keyword %s to index %d\n", c->keywords[i], *smap_get(c->keyword_indexer, c->keywords[i]));
            printf("index %d to keyword %s\n ", i, c->keywords[i]);
            
            
            
            // check to make sure everything has an array WORKS
            printf("%s vector at instantiation: [", c->keywords[i]);
            int * vector = smap_get(c->table, c->keywords[i]);
            for(int j=0; j < n; j++) {
                printf("%d ", vector[j]);
            }
            printf("]\n");
            
            
        }
        printf("n: %d\n", c->n);
        
        
        // test the cooccur_get_vector
        double * row = cooccur_get_vector(c, c->keywords[0]);
        for(int i = 0; i < c->n; i++) {
            printf("%lf\n", row[i]);
        }
        */
        
        // create initial context
        int context_size = 0;
        char ** context = cooccur_read_context(c, stdin, &context_size);
        
        while(context != NULL) {
            cooccur_update(c, context, context_size);
            
            /*
            // WORKING
            // print what the matrix looks like now
            for(int i = 0; i < c->n; i++) {
                printf("keyword: %s\n ", c->keywords[i]);
                int * row = smap_get(c->table, c->keywords[i]);
                double * drow = cooccur_get_vector(c, c->keywords[i]);
                printf("row: [");
                for (int j = 0; j < c->n; j++) {
                    printf("%d ", row[j]);
                }
                printf("]\n");
                printf("drow: [");
                for (int j = 0; j < c->n; j++) {
                    printf("%lf ", drow[j]);
                }
                printf("]\n");
                free(drow);
            }
            */
            
            // free previous context
            for(int i = 0; i < context_size; i++) {
                free(context[i]);
            }
            free(context);

            // reset for new context    
            context_size = 0;
            context = cooccur_read_context(c, stdin, &context_size);
        }
        
        
        // print what the matrix looks like now
        for(int i = 0; i < c->n; i++) {
            
            /*
            printf("keyword: %s\n ", c->keywords[i]);
            int * row = smap_get(c->table, c->keywords[i]);
            printf("row: [");
            for (int j = 0; j < c->n; j++) {
                printf("%d ", row[j]);
            }
            printf("]\n");
            */
            
            double * drow = cooccur_get_vector(c, c->keywords[i]);
            printf("%s: [", c->keywords[i]);
            for (int j = 0; j < c->n - 1; j++) {
                printf("%lf, ", drow[j]);
            }
            printf("%lf]\n", drow[c->n-1]);
            free(drow);
        }
        
        
        // printf("Stage 14\n");
        
        // free cooccurrence_matrix
        cooccur_destroy(c);
        
        // freeing keywords
        for(int i = 0; i < (argc - 1); i++) {
            free(keywords[i]);
        }
        free(keywords);
    }
    
    return 1;
}
