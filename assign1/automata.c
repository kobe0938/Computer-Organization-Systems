#include <errno.h>
#include <error.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "samples/prototypes.h"

#define INVERTED_BOX "\e[7m \e[0m"

/* uncomment below to swap in "+" as definition of LIVE_STR if your
 * terminal doesn't play nice with inverted box
 */
#define LIVE_STR "+"
#define EMPTY_STR " "
#define DEFAULT_GEN_0 (1L << 32)
#define N_GENERATIONS 32
const int long_shift_length = 63;
const int long_length = 64;

/* FUNCTION: load_index
 * ------------------------
For normal case, take in the temp, i, mask and cur_gen, 
and output the index of the ith position of the next line.
 */
unsigned int load_index(unsigned int temp, int i, unsigned long mask, unsigned long cur_gen) {
    unsigned int res = 0;
    mask <<= 1;
    temp = (mask & cur_gen) >> (i + 1);
    res |= (temp << 2);
    mask >>= 1;
    temp = (mask & cur_gen) >> (i);
    res |= (temp << 1);
    mask >>= 1;
    temp = (mask & cur_gen) >> (i - 1);
    res |= temp;
    return res;
}

/* FUNCTION: load_index_63
 * ------------------------
For the beginning case(i = 63), take in the temp, i, mask and cur_gen, 
and output the index of the ith position of the next line.
 */
unsigned int load_index_63(unsigned int temp, int i, unsigned long mask, unsigned long cur_gen) {
    temp |= (mask & cur_gen) >> i;
    temp <<= 1;
    mask >>= 1;
    temp |= (mask & cur_gen) >> (i - 1);
    return temp;
}

/* FUNCTION: load_index_0
 * ------------------------
For the ending case(i = 0), take in the temp, i, mask and cur_gen, 
and output the index of the ith position of the next line.
 */
unsigned int load_index_0(unsigned int temp, int i, unsigned long mask, unsigned long cur_gen) {
    mask <<= 1;
    temp |= (mask & cur_gen) >> (i + 1);
    temp <<= 1;
    mask >>= 1;
    temp |= (mask & cur_gen) >> i;
    return temp << 1;
}

/* FUNCTION: advance
 * ------------------------
Take in the previous line of binary value, and the corrsponding rule. Output the next line of binary value.
 */
unsigned long advance(unsigned long cur_gen, unsigned char ruleset) {
/*NOTE for TAs and instructor: I am aware of that this problem could not be just limited 
in the bit operations.(like problem 1) But I want to polish my skills to use bit operations. 
Therefore, the code sould be a little longer than using not only bit operation ones.*/
    unsigned long binary_result = 0;
    unsigned int index = 0;
    unsigned int temp = 0;
    unsigned long mask = 1L << long_shift_length;
    unsigned char ruleset_copy = ruleset;
    for (int i = long_shift_length; i > -1; i--) {
        if (i != 0 && i != long_shift_length) {
            index = load_index(temp, i, mask, cur_gen);
        } else if (i == long_shift_length) {
            index = load_index_63(temp, i, mask, cur_gen);
        } else {
            index = load_index_0(temp, i, mask, cur_gen);
        }
        mask >>= 1;
        temp = 0;
        binary_result |= (((ruleset_copy >> index) & 1L) << i);
        // here the L behind 1 is important, could not be missing
        ruleset_copy = ruleset;
    }
    return binary_result;
}

/* FUNCTION: dear_generation
 * ------------------------
Take in a line of binary value and output the drawing of it.
 */
void draw_generation(unsigned long gen) {
    // each cell in gen is drawn as LIVE_STR or EMPTY_STR depending on whether cell is on or off
    unsigned long mask = 1L << long_shift_length;
    for (int i = 0; i < long_length; i++) {
        if ((gen & mask) != 0) {
            printf("%s", LIVE_STR);
        } else {
            printf("%s", EMPTY_STR);
        }
        mask = mask >> 1;
    }
    printf("\n");
}




// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------

/* FUNCTION: convert_arg
 * ------------------------
Take in the str, convert it into a long, then output.
 */
unsigned long convert_arg(const char *str, unsigned long low, unsigned long high, const char *argname) {
    char *end;
    unsigned long n = strtoul(str, &end, 0);
    if (*end != '\0') {
        error(1, 0, "Invalid number '%s' for %s", str, argname);
    }
    if (n < low || n > high || errno == ERANGE) {
        error(1, 0, "%s %s is not in range [%#lx, %#lx]", argname, str, low, high);
    }
    return n;
}
    
/* FUNCTION: main
 * ------------------------
Take in the argv and call each function so as to draw the automation graph.
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        error(1, 0, "Missing argument. Please supply ruleset and optional initial generation.");
    }

    unsigned long gen_0 = DEFAULT_GEN_0;
    unsigned char ruleset = convert_arg(argv[1], 0, UCHAR_MAX, "ruleset");
    if (argc > 2) {
        gen_0 = convert_arg(argv[2], 0, ULONG_MAX, "initial generation");
    }

    if (ruleset == 0) {
        draw_generation(gen_0);
    } else {
        unsigned long cur_gen = gen_0;
        for (int i = 0; i < N_GENERATIONS; i++) {
            draw_generation(cur_gen);
            unsigned long next_gen = advance(cur_gen, ruleset);
            if (next_gen == cur_gen) {
                break;
            }
            cur_gen = next_gen;
        }
    }
    return 0;
}
