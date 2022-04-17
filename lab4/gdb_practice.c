/*
 * CS107
 * Lab 4
 *
 * This file contains example code for learning gdb.
 * See the lab 4 writeup for more information.
 */

#include <stdlib.h>
#include <stdio.h>

void my_function(int *arr, size_t nelems) {
    for (int i = 0; i < nelems; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void my_function_generic(void *ptr, size_t nelems) {
    printf("ptr = %p, nelems = %ld\n", ptr, nelems);
}

int main(int argc, char *argv[]) {
    int nums[] = {0, 0xffffffff, 12345, 0x11111111, 25};
    int nelems = sizeof(nums) / sizeof(nums[0]);

    my_function(nums, nelems);
    my_function_generic(nums, nelems);

    return 0;
}
