/*
This file takes in the key, base and p_nelem, which is the number of 
elements, and the width, which is the bytes of a single elements, and a 
compare function. And search for the key in the base. If not detect the
key in the base, then add it to the base array. Update the p_nelem.
*/
#include <string.h>
#include "samples/prototypes.h"

void *binsert(const void *key, void *base, size_t *p_nelem, size_t width,
              int (*compar)(const void *, const void *)) {
    // void *original_last = (char *)base + (*p_nelem - 1) * width;
    void *ori_base = base;
    for (size_t nremain = *p_nelem; nremain != 0; nremain >>= 1) {
        void *p = (char *)base + (nremain >> 1) * width;
        int sign = compar(key, p);
        if (sign == 0) {
            return p;
        }
        if (sign > 0) {  /* key > p: move right */
            base = (char *)p + width;
            nremain--;
        }       /* else move left */
    }
    size_t n = *p_nelem - ((char *)base - (char *)ori_base)/width;
    // fisrt to make some(a width) place for the new element
    memmove((char *)base + width, base, n * width);
    // then add the new elemwnt
    memcpy(base, key, width);
    (*p_nelem)++;
    return base;
}
