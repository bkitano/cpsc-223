#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "point.h"
#include "plist.h"

int main(int argc, char **argv) {
    
    plist * p = plist_create();
    
    int n;
    fscanf(stdin, "%d", &n);
    
    p->size = n;
    
    fprintf(stdout, "n: %d\n", n);
    
    for(int i = 0; i < n; i++) {
        double x;
        double y;
        
        fscanf(stdin, "%lf %lf \n", &x, &y);
        
        point t;
        t.x = x;
        t.y = y;
        
        // not adding correctly
        printf("%lf %lf \n", t.x, t.y);
        
        bool success = plist_add_end(p, &t);
        printf("%d\n", success);
    }
    
    plist_fprintf(stdout, "%.3f\n", p);
    

}