#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "isset.h"

int main(int argc, char **argv) {
    
    isset * c = isset_create();
    printf("%d\n", isset_size(c));
    isset_destroy(c);
}


