/* File: prototypes.h
 * ------------------
 * Defines the required interface for the functions of CS107 assign1.
 * Refer to assignment writeup for more information.
 */

#ifndef _prototypes_h
#define _prototypes_h


long signed_max(int bitwidth);
long signed_min(int bitwidth);
long sat_add(long operand1, long operand2, int bitwidth);


void draw_generation(unsigned long gen);
unsigned long advance(unsigned long gen, unsigned char ruleset);


int to_utf8(unsigned short code_point, unsigned char utf8_bytes[]);


#endif
