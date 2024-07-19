#include <stdlib.h>
#include <stdio.h>
#include <float.h>

double fabs (double value) {
    return value<0.0 ? -value : value;
}

double power(double num, int n){
    double ans = 1.0;
    for(int i = 0; i < n; i++){
        ans = ans * num;
    }
    return ans;
}

int main(int argc, char *argv[]) {

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // The problem we are trying to solve is:
    // r^n = x
    // given n and x, find r.
    // n is a positive integer larger than 0.
    // x is a positive floating point value.
    // https://en.wikipedia.org/wiki/Nth_root
    // r needs to be found such that r is within the answer value specified by precision.

    double x;
    fscanf(fp, "%lf", &x);

    size_t n;
    fscanf(fp, "%ld", &n);

    double precision;
    fscanf(fp, "%lf", &precision);

    // The easiest way to solve this in C is to invoke the pow() function as follows.
    // Unfortunately, as part of this assignment we are not allowing importing math.h.
    // Even if you import math.h, that Makefile does not link the math library using the -lm flag.
    // So, you will need to write a separate algorithm to find the n-th root.
    // printf("%f", pow(x, 1.0/n));

    // There are many algorithms that can find the n-th root (Newton-Raphson, etc.).
    // A straightfoward one for our purposes is a binary search along the number line.
    // You should review how the binary search algorithm works.

    double error = DBL_MAX;
    double low = 1.0;
    double high = x;
    if(x < 1.0){
        high = 1.0;
        low = x;
    }

    while ( precision<(error) ) {
        double mid = (low + high)/2.0;
        if(fabs(power(mid, n)) < x){
            low = mid;
        }else{
            high = mid;
        }
        error = (high-low);
    }

    printf("%.16f\n",low); // print with default precision

    return EXIT_SUCCESS;

}
