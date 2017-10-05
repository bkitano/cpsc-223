#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {

    float x;
    float y;
    fscanf(stdin, "%f %f", &x, &y);
    fprintf(stdout, "%f", x+y);
}