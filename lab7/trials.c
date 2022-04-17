/* File: trials.c
 * --------------
 * Various ways of computing 2^power to be observed for instruction count
 */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


// Use the built in library function
long two_to_power_A(unsigned int exp) {
     return pow(2, exp);
}

// is specialized 2^exp less work than general pow()?
long two_to_power_B(unsigned int exp) {
    return exp2(exp);
}

// loop to repeatedly multiply by 2, iterates power # of times
long two_to_power_C(unsigned int exp) {
    long result = 1;
    for (int i = 1; i <= exp; i++) {
        result *= 2;
    }
    return result;
}

// base is known to be 2, simple bit shift in binary is perfect!
long two_to_power_D(unsigned int exp) {
    return 1L << exp;
}

/* Configuration code to run different functions */

static struct {
    long (*fn)(unsigned int);
    char *descr;
} pfns[] =
   {{two_to_power_A, "calls pow()"},
    {two_to_power_B, "calls exp2()"},
    {two_to_power_C, "multiply loop",},
    {two_to_power_D, "bit shift"}};
static const int npfns = sizeof(pfns) / sizeof(*pfns);


double warmup_lib(double d) {
    // resolve lib before counting
    return floorf(pow(exp2(d), log2f(d)));
}

int main(int argc, char *argv[]) {
    warmup_lib(rand());

    long result = 0;
    int input = rand();

    for (int i = 0; i < npfns; i++) {
        result += pfns[i].fn(input % 60);
    }

    return result == 0;
}

