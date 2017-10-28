#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "smap.h"

int hash1(const char word[]) {
    printf("%s\n", word);
    
    char *i;
    i = &word[0];
    
    printf("test: %c\n", *i);
    
    int l = strlen(word);
    
    printf("length: %d\n", l);
    
    for(int i = 0; i < l; i++) {
        printf("%dth ascii: %d\n",i, word[i]);
    }
    return 1;
}

int main(int argc, char **argv) {
    // hash1("asdf");
    
    smap * test;
    test = smap_create(hash1);
    printf("%d\n", smap_size(test));
    
    return 1;
}