#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "point.h"
#include "plist.h"
#include "closest_pair.h"

int main(int argc, char **argv) {
    
    // fscanf test
    /*
    plist * p = plist_create();
    
    int n;
    fscanf(stdin, "%d", &n);
    
    p->size = n;
    
    fprintf(stdout, "n: %d\n", n);
    
    for (int i = 0; i < n; i++) {
        double x;
        double y;
        
        fscanf(stdin, "%lf %lf \n", &x, &y);
        
        point t;
        t.x = x;
        t.y = y;
        
        bool success = plist_add_end(p, &t);
        printf("%d\n", success);
    }
    
    plist_fprintf(stdout, "%.3f\n", p);
    */
    
    point a = {1, 1};
    point b = {-1, 2};
    point c = {0, -4};
    
    plist * w = plist_create();
    
    plist_add_end(w, &a);
    plist_add_end(w, &b);
    plist_add_end(w, &c);
    
    point p1;
    point p2;
    double d;
    
    closest_pair_brute_force(w, &p1, &p2, &d);
    
    point_fprintf(stdout, "%.3f\n", &p1);
    point_fprintf(stdout, "%.3f\n", &p2);
    printf("%.3f\n", d);
    
    
    return 1;

}