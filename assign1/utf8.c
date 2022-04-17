#include <error.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "samples/prototypes.h"

const int move_10 = 0x6;
const int move_110 = 0x5;
const int move_1110 = 0x4;
const int move_first_byte = 0xc;
const int move_second_byte = 0x6;
const char hex_1110 = 0xe;
const char hex_110 = 0x6;
const char hex_10 = 0x2;
const char initial_mask_0 = 0x7F;
const short bin_1 = 0x7F;
const short bin_2 = 0x7FF;

/* FUNCTION: to_utf8
 * ------------------------
Take in the hex format and convert it to 1-3 bytes according to certain rule. Output the bytes length and modify it 
in the utf8_bytes[].
 */
int to_utf8(unsigned short code_point, unsigned char utf8_bytes[]) {
    unsigned char mask_1110_on = hex_1110;
    unsigned char mask_110_on = hex_110;
    unsigned char mask_10_on = hex_10;
    unsigned char mask_0 = initial_mask_0;    // 0111 1111
    unsigned char mask_off = 1;    // 0000 0001
    unsigned int bytes_length = 1;
    if (code_point <= bin_1) {
        utf8_bytes[0] = code_point & mask_0;
    } else if (code_point <= bin_2) {
        utf8_bytes[1] = (code_point | (mask_10_on << move_10)) & ~(mask_off << move_10);
        utf8_bytes[0] = ((code_point >> move_second_byte) | (mask_110_on << move_110)) & ~(mask_off << move_110);
        bytes_length = 2;
    } else {
        utf8_bytes[2] = (code_point | (mask_10_on << move_10)) & ~(mask_off << move_10);
        utf8_bytes[1] = ((code_point >> move_second_byte) | (mask_10_on << move_10)) & ~(mask_off << move_10);
        utf8_bytes[0] = ((code_point >> move_first_byte) | (mask_1110_on << move_1110)) & ~(mask_off << move_1110);
        bytes_length = 3;
    }
    return bytes_length;
}

// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------

/* FUNCTION: print_utf8
 * ------------------------
A helper function to format the output.
 */
void print_utf8(unsigned short code_point, unsigned char utf8_bytes[], int utf8_bytes_length) {
    int i;
    printf("U+%04X   UTF-8 Hex:", code_point);
    for (i = 0; i < utf8_bytes_length; i++) {
       printf(" %02x", utf8_bytes[i]);
    }
    for ( ; i < 4; i++) {
        printf("   ");
    }
    utf8_bytes[utf8_bytes_length] = '\0';
    printf("Character: %s  \n", utf8_bytes);
}
/* FUNCTION: convert_arg
 * ------------------------
Take in the str, output the converted short.
 */
unsigned short convert_arg(const char *str, unsigned short low, unsigned short high) {
    char *end;
    long n = strtol(str, &end, 0);
    if (*end != '\0') {
        error(1, 0, "Invalid code point '%s'", str);
    }
    if (n < low || n > high) {
        error(1, 0, "Code point %s not in range [%u, %u]", str, low, high);
    }
    return n;
}

/* FUNCTION: main
 * ------------------------
Take in the hex  and call each function so as to output the corrsponding letter in Unicode.
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        error(1, 0, "Missing argument. Please supply one or more unicode code points in decimal or hex.");
    }

    for (int i = 1; i < argc; i++) {
        unsigned short code_point = convert_arg(argv[i], 0, USHRT_MAX);
        unsigned char utf8_bytes[4];
        int num_utf8_bytes = to_utf8(code_point, utf8_bytes);
        print_utf8(code_point, utf8_bytes, num_utf8_bytes);
    }
    return 0;
}
