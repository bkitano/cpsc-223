#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "smap.h"


int hash1(const char * word) {
    
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


int main(int argc, char **argv) {
    
    printf("before smap declaration\n");
    smap * m = smap_create(hash1);
    
    char * key = "asdf";
    int value[] = {1,2,3};
    
    printf("before smap put\n");
    smap_put(m, key, value);
    
    printf("size: %d\n", smap_size(m));
    
    return 1;
}