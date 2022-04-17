/* This file is a implicit heap allocation which has eaders that track block information (size, status in-use or free) -
that is 8 bytes big, using any of the least significant 3 bits to store the status.
Free blocks that are recycled and reused for subsequent malloc requests if possible.
A malloc implementation that searches the heap for free blocks via an implicit list (i.e. traverses block-by-block).
*/
#include <stdio.h>
#include <string.h>
#include "./allocator.h"
#include "./debug_break.h"

#define HEADER_SIZE 8
#define BUFFER_SIZE 5

static void *segment_start;  // mark the start of this heap
static size_t segment_size;
static size_t nheaders = 0;  // number of headers used

const int status_free = 0x0;
const int status_used = 0x1;
const size_t for_61_mask = ~0x7;
const size_t for_3_mask = 0x7;
// if the last three bits of a header is '000', it means free
// else if the last three bits is '001,' it means used
size_t start_header;

size_t cur_header;
void *cur_position;
size_t first_61_bits;
size_t last_3_bits;
char used_or_free[BUFFER_SIZE];

/*This function must be called by a client before making any allocation requests.  The function returns true if 
initialization was successful, or false otherwise. The myinit function can be called to reset the heap to an empty state
. When running against a set of of test scripts, our test harness call myinit before starting each new script.
*/
bool myinit(void *heap_start, size_t heap_size) {
    if (heap_start == NULL) {
        return false;
    }
    segment_start = heap_start;
    segment_size = heap_size;
    // minus the size of header itself and we want to right shift 3
    start_header = (heap_size - HEADER_SIZE);
    
    // bytes_used = HEADER_SIZE;
    // max_free_size = (heap_size - HEADER_SIZE);

    *(size_t *)segment_start = start_header;
    
    nheaders = 1;
    return true;
}

/* This function rouldup the sz to mutiple of mult.
 */
size_t roundup(size_t sz, size_t mult) {
    return (sz + mult - 1) & ~(mult - 1);
}

/* This function assgins some space that is equal to requested_size and return its first position as a void 
pointer.
 */
void *mymalloc(size_t requested_size) {
    // TODO(you!): remove the line below and implement this!
    if (requested_size == 0 || requested_size >= MAX_REQUEST_SIZE) {
        return NULL;
    }
    
    size_t needed = roundup(requested_size, ALIGNMENT);

    cur_header = *(size_t *)segment_start;
    cur_position = segment_start;
    first_61_bits = cur_header & for_61_mask;
    last_3_bits = cur_header & for_3_mask;

    
    void* ptr = NULL;
    for (size_t i = 0; i < nheaders; i++) {
        if (last_3_bits == 0) {
            if (first_61_bits >= needed) {
                if (needed >= first_61_bits - HEADER_SIZE) {
                    cur_header = cur_header | status_used;
                    *((size_t *)cur_position) = cur_header;
                    ptr = (char *)cur_position;
                } else {
                    size_t original_size = first_61_bits;
                    cur_header = needed | status_used;
                    *((size_t *)cur_position) = cur_header;
                    ptr = (char *)cur_position;
                    cur_position = (char *)cur_position + needed + HEADER_SIZE;
                    *((size_t *)cur_position) = original_size - HEADER_SIZE - needed;
                    nheaders += 1;
                }
                return (char *)ptr + HEADER_SIZE;
                break;
            }
        }
        if (i == nheaders - 1) {
            break;
        }
        cur_position = (char *)cur_position + first_61_bits + HEADER_SIZE;
        cur_header = *(size_t *)cur_position;
        first_61_bits = cur_header & for_61_mask;
        last_3_bits = cur_header & for_3_mask;
    }
    return NULL;
}

/* This function frees the heap space of the pointer ptr.
 */
void myfree(void *ptr) {
    // TODO(you!): implement this!
    if (ptr == NULL) {
        return;
    }
    cur_header = *(size_t *)((char*)ptr - HEADER_SIZE);
    first_61_bits = cur_header & for_61_mask;
    *(size_t *)((char *)ptr - HEADER_SIZE) = first_61_bits;
}

/* This function does not do in-place realloc, it would always free(ptr) and malloc(ptr) in new sapce.
 */
void *myrealloc(void *old_ptr, size_t new_size) {
    // TODO(you!): remove the line below and implement this!
    if (new_size == 0) {
        myfree(old_ptr);
        return NULL;
    }
    if (old_ptr == NULL) {
        return mymalloc(new_size);
    }
    size_t old_size = *((size_t *)old_ptr - 1) & for_61_mask;
    void *new_ptr = mymalloc(new_size);
    if (new_size >= old_size) {
        memcpy(new_ptr, old_ptr, old_size);
    } else {
        memcpy(new_ptr, old_ptr, new_size);
    }
    myfree(old_ptr);
    return new_ptr;
}

/* Return true if all is ok, or false otherwise.This function is called periodically by the testharness to check the 
state of the heap allocator. It tests the segment_start is not NULL and segment size is not 0. As well as the sum of
block size should be segment size.
     */
bool validate_heap() {
    if (segment_start == NULL) {
        printf("The heap has a NULL start!\n");
        breakpoint();
        return false;
    }
    if (segment_size == 0) {
        printf("The heap has size of 0!\n");
        breakpoint();
        return false;
    }
    size_t cur_header_dump = *(size_t *)segment_start;
    void *cur_position_dump = segment_start;
    size_t first_61_bits_dump = cur_header_dump & for_61_mask;
    size_t sum = 0;
    for (size_t i = 0; i < nheaders; i++) {
        sum = sum + HEADER_SIZE + first_61_bits_dump;
        if (i == nheaders - 1) {
            break;
        }
        cur_position_dump = (char *)cur_position_dump + first_61_bits_dump + HEADER_SIZE;
        cur_header_dump = *(size_t *)cur_position_dump;
        first_61_bits_dump = cur_header_dump & for_61_mask;
    }
    if (sum != segment_size) {
        breakpoint();
        return false;
    }
    return true;
}

/* This function prints the structure of the heap.
 */
void dump_heap() {
    printf("Heap segment starts at address %p, ends at %p, %lu headers currently used.\n",
           segment_start, (char *)segment_start + segment_size, nheaders);

    size_t cur_header_dump = *(size_t *)segment_start;
    void *cur_position_dump = segment_start;
    size_t first_61_bits_dump = cur_header_dump & for_61_mask;
    size_t last_3_bits_dump = cur_header_dump & for_3_mask;

    for (size_t i = 0; i < nheaders; i++) {
        if (last_3_bits_dump == 1) {
            strcpy(used_or_free, "used");
        } else {
            strcpy(used_or_free, "free");
        }
        printf("The %lu header: %lu bytes %s\n",
           i + 1, first_61_bits_dump, used_or_free);
        if (i == nheaders - 1) {
            break;
        }
        cur_position_dump = (char *)cur_position_dump + first_61_bits_dump + HEADER_SIZE;
        cur_header_dump = *(size_t *)cur_position_dump;
        first_61_bits_dump = cur_header_dump & for_61_mask;
        last_3_bits_dump = cur_header_dump & for_3_mask;
    }
}
