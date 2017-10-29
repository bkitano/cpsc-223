#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "smap.h"


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
    
    return (99991 * sum);
}

void keywords_seen(char **keywords_seen, int (*hash)(const char *s), int n_keywords_total, char *line, int *n) {
    char *token;
    smap * local = smap_create(hash);
    
    /* get the first token */
    token = strtok(line, " ");
    
    *n = 0;
    
    /* walk through other tokens */
    while( token != NULL ) {
        // printf( " %s\n", token );
        
        if(!smap_contains_key(local, token)) { // checks for redundancies
            int * dummy = malloc(sizeof(int));
            *dummy = 1;
            smap_put(local, token, dummy);
            keywords_seen[*n] = token;
            *n++;
        }
        
        token = strtok(NULL, " ");
    }
    
    for(int i = 0; i < *n; i++) {
        printf("%s\n", keywords_seen[i]);
    }
    
    smap_destroy(local);
}

int main(int argc, char **argv) {
    
    char str[80] = "a b a a c d e f g g h i a f a";
    int n_keywords_total = 5;
    char ** keywords = malloc(sizeof(char *) * n_keywords_total);
    int keywords_length;
    
    keywords_seen(keywords, hash, n_keywords_total, str, &keywords_length);
    
    return 0;
}