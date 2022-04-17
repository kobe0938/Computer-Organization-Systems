/* File: round.c
 * -------------
 * Code passages to read as part of lab1.
 */

#include <stdbool.h>
#include <stdio.h>

// From lecture
bool is_power_of_2(unsigned int num) {
    return num != 0 && (num & (num - 1)) == 0;
}

/* What is special about powers of 2 that allow rounding via bitwise ops
 * in place of the more expensive multiply/divide used for general case?
 */
unsigned int round_up(unsigned int value, unsigned int multiple) {
    if (!is_power_of_2(multiple)) {
        return ((value + multiple - 1) / multiple) * multiple;
    } else {
        return (value + multiple - 1) & ~(multiple - 1);
    }
}

void print_round_up(unsigned int value, unsigned int multiple) {
    printf("round_up(%u, %u) = %u\n", value, multiple, round_up(value, multiple));
}

int main(int argc, char *argv[]) {
    print_round_up(2, 7);
    print_round_up(6, 5);
    print_round_up(100, 32);
    print_round_up(4, 4);
    return 0;
}

