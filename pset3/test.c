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
    
    /*
    point a = {1, 1};
    point b = {-1, 2};
    point c = {0, -4};
    point dasdf = {3, 5};
    point noah = {7, 9};
    */
    
    // order x: [{-1,2}{0,-4}{1,1}{3,5}]
    // order y: {{0,-4}{1,1}{-1,2}{3,5}}

/*
    plist * w = plist_create();
    
    plist_add_end(w, &a);
    plist_add_end(w, &b);
    plist_add_end(w, &c);
    plist_add_end(w, &dasdf);
    plist_add_end(w, &noah);
  */
  
    /* TESTED 10.08.18:2130
    // brute force test
    
    point p1;
    point p2;
    double d;
    
    closest_pair_brute_force(w, &p1, &p2, &d);
    
    point_fprintf(stdout, "%.3f\n", &p1);
    point_fprintf(stdout, "%.3f\n", &p2);
    printf("%.3f\n", d);
    */
    
    /* TESTED 10.08.17:2133
    // split_list_x test
    
    plist_sort(w, point_compare_x);
    
    plist * left = plist_create();
    plist * right = plist_create();
    
    split_list_x(w, left, right);
    
    plist_fprintf(stdout, "w: %.3f\n", w);
    plist_fprintf(stdout, "l: %.3f\n", left);
    plist_fprintf(stdout, "r: %.3f\n", right);
    */
    
    /* TESTED 10.08.17:2136
    // copy_list test
    
    plist * v = plist_create();
    copy_list(v, w);
    plist_fprintf(stdout, "%.3f\n", v);
    */
    
    /* TESTED 10.08.17:2140
    // search_middle test
    
    point p1;
    point p2;
    double d;
    
    search_middle(w, &p1, &p2, &d);
    
    point_fprintf(stdout, "%.3f\n", &p1);
    point_fprintf(stdout, "%.3f\n", &p2);
    printf("%.3f\n", d);
    */
    
    /* TESTED 10.08.17:2305
    // read_points test
    plist * v = plist_create();
    
    int n;
    fscanf(stdin, "%d", &n);
    
    v->size = n;
    
    
    read_points(stdin, v, n);
    
    plist_fprintf(stdout, "%.3f\n", v);
    return 1;
    */
    
    /* 
    // make middle test
    
    plist * list_x = plist_create();
    plist * list_y = plist_create();
    
    int n;
    fscanf(stdin, "%d", &n);
    
    read_points(stdin, list_x, n);
    
    plist_sort(list_x, point_compare_x);
    
    copy_list(list_y, list_x);
    plist_sort(list_y, point_compare_y);
    
    double left = 5;
    double right = 10;
    
    plist * middle = plist_create();
    make_middle(list_y, middle, left, right);
    // should get 6 outputs
    
    plist_fprintf(stdout, "%.3f\n", middle);
    * /

}