#include <errno.h>
#include <error.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "samples/prototypes.h"


long signed_max(int bitwidth) {
    long max = 0;
    max = 1 << (bitwidth - 1);
    max -= 1;
    return max;
}

long signed_min(int bitwidth) {
    return - (signed_max(bitwidth) + 1);
}

long sat_add(long operand1, long operand2, int bitwidth) {
    long result = operand1 + operand2;
    long final_result = result;
    if (operand1 >> (bitwidth - 1) == 0 && operand2 >> (bitwidth - 1) == 0) {
        // which means they are all positive, could exist positive max overflow
        if (result >> (bitwidth - 1) != 0) {
            final_result =  signed_max(bitwidth);
        }
    } else if (operand1 >> (bitwidth - 1) != 0 && operand2 >> (bitwidth - 1) != 0) {
        // which means they are all negative, could exist negative min overflow
        if ((-result) >> (bitwidth - 1) != 0) {
            final_result =  signed_min(bitwidth);
        }
    } 
    // one positive + one negative -> would never overflow
    return final_result;
}



// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------

/* FUNCTION: convert_arg
 * ------------------------
Convert the string into a long type;
 */
long convert_arg(const char *str, long low, long high, const char *argname) {
    char *end;
    long n = strtol(str, &end, 0);
    if (*end != '\0') {
        error(1, 0, "Invalid number '%s'", str);
    }
    if (n < low || n > high || errno == ERANGE) {
        error(1, 0, "Illegal value %s. %s must be in range [%ld, %ld]", str, argname, low, high);
    }
    return n;
}
/* FUNCTION: main
 * ------------------------
Two choices:
1.Input 2 argv(including one sat) to display the bit range of the second input.
eg. ./sat 4
4-bit signed integer range
min: -8   0xfffffffffffffff8
max:  7   0x0000000000000007

2.Input 4 agrv(including one sat) to calculate the sum of the third and fourth argv within
the second argv's bit range.
eg. ./sat 8 126 4
Matched output: 126 + 4 = 127
The overflow will be considered. The negative input and the minimum negative overflow will
be considered as well.
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        error(1, 0, "Missing argument. Please specify the bitwidth.");
    }

    int bitwidth = convert_arg(argv[1], 4, sizeof(long) * CHAR_BIT, "Bitwidth");
    long min = signed_min(bitwidth);
    long max = signed_max(bitwidth);

    if (argc < 4)  {
        printf("%d-bit signed integer range\n", bitwidth);
        printf("min: %ld   %#018lx\n", min, min);
        printf("max:  %ld   %#018lx\n", max, max);
    } else {
        long sum = convert_arg(argv[2], min, max, "Operand");
        printf("%ld ", sum);
        for (int i = 3; i < argc; i++) {
            long next = convert_arg(argv[i], min, max, "Operand");
            printf("+ %ld ", next);
            sum = sat_add(sum, next, bitwidth);
        }
        printf("= %ld\n", sum);
    }
    return 0;
}

