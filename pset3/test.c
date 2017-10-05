#include <stdlib.h>
#include <stdio.h>

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
        
        point_fprintf(stdout, "%.3f\n", &t);
        
        // printf("%lf, %lf \n", x, y);
    }
}