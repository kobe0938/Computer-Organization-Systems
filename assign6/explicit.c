/* This function is an explicit heap allocator. Its block headers and recycling freed nodes as specified in the implicit
 implementation. An explicit free list managed as a doubly-linked list, using the first 16 bytes of each free block's 
payload for next/prev pointers. Note that the header should not be enlarged to add fields for the pointers. Since the 
list only consists of free blocks, the economical approach is to store those pointers in the otherwise unused payload. 
Malloc searchs the explicit list of free blocks. A freed block should be coalesced with its neighbor block to the right 
if it is also free. Realloc resizes a block in-place whenever possible. Even if an in-place realloc is not possible, it
 absorbs adjacent free blocks as much as possible until it either can realloc in place, or can no longer absorb and must
 reallocate elsewhere.
*/
#include <stdio.h>
#include <string.h>
#include "./allocator.h"
#include "./debug_break.h"

#define HEADER_SIZE 8
#define ALIGNMENT_16 16
#define BUFFER_SIZE 5

static size_t *segment_start;
static size_t segment_size;
static size_t nheaders = 0;

const int status_free = 0x0;
const int status_used = 0x1;
const size_t for_61_mask = ~0x7;
const size_t for_3_mask = 0x7;

size_t first_61_bits;
size_t last_3_bits;
char used_or_free[BUFFER_SIZE];

typedef struct dll {
    size_t *prev;
    size_t *next;
} dll;
size_t *ffh = NULL;

size_t nfreeheaders = 0;

/* This must be called by a client before making any allocation requests.  The function returns true if initialization 
was successful, or false otherwise. The myinit function can be called to reset the heap to an empty state. When running 
against a set of of test scripts, our test harness calls myinit before starting each new script.
     */
bool myinit(void *heap_start, size_t heap_size) {
    if (heap_start == NULL) {
        return false;
    }
    segment_start = (size_t *)heap_start;
    segment_size = heap_size;
    *segment_start = (heap_size - HEADER_SIZE);

    nheaders = 1;
    ffh = segment_start;
    dll dll_1;
    dll_1.prev = NULL;
    dll_1.next = NULL;
    *(dll *)(ffh + 1) = dll_1;
    
    nfreeheaders = 1;
    return true;
}

/* This function roundup the sz to mutiple of mult.
 */
size_t roundup(size_t sz, size_t mult) {
    return (sz + mult - 1) & ~(mult - 1);
}

/* meaning free whole of a used block, ptr points to the header
 */
void add_free_block(size_t *ptr) {
    dll dll_1;
    // case 1: there is no other free blocks
    if (ffh == NULL) {
        dll_1.prev = NULL;
        dll_1.next = NULL;
        nfreeheaders = 1;
        *(dll *)(ptr + 1) = dll_1;
        ffh = ptr;
        return;
    }
        
    // case 2: not the only one
    // change prev to ptr, the following line is necessary
    *(ffh + 1) = (size_t)ptr;
    // change things at ptr
    dll_1.prev = NULL;
    dll_1.next = ffh;
    *(dll *)(ptr + 1) = dll_1;
    nfreeheaders += 1;

    // change header
    *ptr = *ptr & for_61_mask;
    // update ffh
    ffh = ptr;
}

/*  meaning use whole of it, but it is originally freed, points to header
 */
void remove_free_block(size_t *ptr) {
    // case 1: only one free block
    if (nfreeheaders == 1) {
        *ptr = *ptr | status_used;
        ffh = NULL;
        nfreeheaders -= 1;
        return;
    }
    dll dll_2;
    dll dll_3;
    // first record the info of ptr
    dll_2 = *(dll *)(ptr + 1);
    // case 2: this is the most front one
    if (dll_2.prev == NULL) {
        dll_3 = *(dll *)(dll_2.next + 1);
        dll_3.prev = NULL;
        *(dll *)(dll_2.next + 1) = dll_3;
        nfreeheaders -= 1;
        // need to change ffh
        ffh = dll_2.next;
        // change header
        *ptr = *ptr | status_used;
        return;
    }
    // case 3: this is the most last one
    if (dll_2.next == NULL) {
        dll_3 = *(dll *)(dll_2.prev + 1);
        dll_3.next = NULL;
        *(dll *)(dll_2.prev + 1) = dll_3;
        nfreeheaders -= 1;
        // change header
        *ptr = *ptr | status_used;
        return;
    }
    // case 4: middle case, first update the front one
    dll_3 = *(dll *)(dll_2.prev + 1);
    dll_3.next = dll_2.next;
    *(dll *)(dll_2.prev + 1) = dll_3;
    // update the next one
    dll_3 = *(dll *)(dll_2.next + 1);
    dll_3.prev = dll_2.prev;
    *(dll *)(dll_2.next + 1) = dll_3;
    nfreeheaders -= 1;
    // change header
    *ptr = *ptr | status_used;
}

/* This function iterate through free list and return the postion of free block.
 */
size_t *find_free_block(size_t size) {
    if (ffh == NULL) {
        return NULL;
    }
    size_t *cur = ffh;
    while (cur != NULL) {
        if ((*cur & for_61_mask) >= size) {
            return cur;
        }
        // IMPORTANT format!!! TO AVOID USING dll
        cur = (size_t *)(*(cur + 2));
    }
    return NULL;
}

// This functin changes the status of a certain header.
void change_header(size_t *ptr, size_t size, int status) {
    if (status == 1) {
        *ptr = size | status;
    } else {
        *ptr = size & for_61_mask;
    }
}

// This first we know that ptr has size > size.
bool need_split(size_t *ptr, size_t size) {
    return ((*ptr - size) >= (ALIGNMENT_16 + HEADER_SIZE));
}

// This function just sets up and call remove and add here, simple!
void split_block(size_t *ptr, size_t size) {
    if (ptr == NULL || ((*ptr & for_3_mask) == 1)) {
        printf("Pass in a valid ptr!\n");
        breakpoint();
    }
    if (size == 0) {
        printf("Do not need to split!\n");
        breakpoint();
    }
    // size here is size required, or size are about to be used,
    // do not have to worry about the size small, already done in find_free
    size_t ori_size = (*ptr);
    size_t *new_ptr = ptr + size/HEADER_SIZE + 1;
    change_header(new_ptr, ori_size - size - HEADER_SIZE, 0);
    change_header(ptr, size, 1);

    // deal with the double linked list!
    // the order could be a problem!
    remove_free_block(ptr);
    add_free_block(new_ptr);
    nheaders += 1;
}


/* The function assigns space for requested_size.
 */
void *mymalloc(size_t requested_size) {
    // TODO(you!): remove the line below and implement this!
    if (requested_size == 0 || requested_size > MAX_REQUEST_SIZE) {
        return NULL;
    }
    // actual needed space
    size_t needed = roundup(requested_size, ALIGNMENT_16);
    size_t *cur = NULL;
    cur = find_free_block(needed);
    // if we could not find any bigger blocks
    if (cur == NULL) {
        return NULL;
    }
    if (need_split(cur, needed)) {
        split_block(cur, needed);
        return (char *)cur + HEADER_SIZE;
    } else {
        change_header(cur, (*cur & for_61_mask), 1);
        remove_free_block(cur);
        return (char *)cur + HEADER_SIZE;
    }
    return NULL;           
}

// This fucntion can not detect non-header ptr(in the middle), which is rare.
bool can_col(size_t *ptr) {
    size_t *next = NULL;
    next = ptr + (*ptr)/HEADER_SIZE + 1;
    if ((char *)next > ((char *)segment_start + segment_size - ALIGNMENT_16 - HEADER_SIZE)) {
        return false;
    }
    return (((*next) & for_3_mask) == 0);
}

// This function coalese two phycial continuous blocks, already can_col.
void col(size_t *ptr) {
    size_t *next = NULL;
    next = ptr + (*ptr)/HEADER_SIZE + 1;
    size_t total = 0;
    // attention here needs to add more 8 bytes
    total = (*ptr & for_61_mask) + *next + HEADER_SIZE;
    remove_free_block(next);
    change_header(ptr, total, 0);
    add_free_block(ptr);
    nheaders -= 1;
}


// This function is a realloc version col, do not write the dll.
void col_r(size_t *ptr) {
    size_t *next = NULL;
    // physical next
    next = ptr + (*ptr)/HEADER_SIZE + 1;
    
    size_t total = 0;
    // attention here needs to add more 8 bytes
    total = (*ptr & for_61_mask) + *next + HEADER_SIZE;
    
    remove_free_block(next);
    change_header(ptr, total, 1);
    
    nheaders -= 1;
}

// This function is a realloc version just sets up and call remove and add here, simple!
void split_block_r(size_t *ptr, size_t size) {
    if (ptr == NULL) {
        printf("Pass in a valid ptr!\n");
        breakpoint();
    }
    if (size == 0) {
        printf("Do not need to split!\n");
        breakpoint();
    }
    // size here is size required, or size are about to be used
    // do not have to worry about the size small, already done in find_free
    size_t ori_size = (*ptr) & for_61_mask;
    size_t *new_ptr = ptr + size/HEADER_SIZE + 1;
    change_header(new_ptr, ori_size - size - HEADER_SIZE, 0);
    change_header(ptr, size, 1);

    // deal with the double linked list!
    // the order could be a problem!
    add_free_block(new_ptr);
    nheaders += 1;
}


// This function free the ptr, and do not have to worry double free.
void myfree(void *ptr) {
    // this points to the header now
    if (ptr == NULL) {
        return;
    }
    size_t *pt = (size_t *)ptr - 1;
    if (can_col(pt)) {
        col(pt);
    } else {
        // can not col, we change header and add_free_block
        change_header(pt, ((*pt) & for_61_mask), 0);
        add_free_block(pt);
    }
}

// This function use in-place realloc whereever possible.
void *myrealloc(void *old_ptr, size_t new_size) {
    if (new_size == 0) {
        myfree(old_ptr);
        return NULL;
    }
    if (old_ptr == NULL) {
        return mymalloc(new_size);
    }
    
    // round up the new_size
    new_size = roundup(new_size, ALIGNMENT_16);
    size_t *cur = (size_t *)old_ptr - 1;
    size_t size = 0;
    size = (*cur) & for_61_mask;
    size_t old_size = size;
    if (new_size <= size) {
        if (need_split(cur, new_size)) {
            split_block_r(cur, new_size);
            return (char *)cur + HEADER_SIZE;
        } else {
            // do not have to do anything, because nothing changed
            return (char *)cur + HEADER_SIZE;
        }
    } else {
        while (can_col(cur)) {
            col_r(cur);
            size = (*cur) & for_61_mask;
            if (new_size <= size) {
                // can NOT use mymalloc here!
                if (need_split(cur, new_size)) {
                    split_block_r(cur, new_size);
                    return cur + 1;
                } else {
                    change_header(cur, (*cur & for_61_mask), 1);
                    return cur + 1;
                }
            }
        }
        // reach this point means can not col enough space;
        void *new_ptr = mymalloc(new_size);
        memcpy(new_ptr, old_ptr, old_size);
        myfree(old_ptr);
        return new_ptr;
    }
}

/* This function tests the segment start is not NULL and teh segment size is not 0. As well as the sum of block size
should be segment size. In the meanwhile, the free double linked list should be vaild.
 */
bool validate_heap() {
    if (segment_start == NULL) {
        printf("The heap has a NULL start!\n");
        breakpoint();
        return false;
    }
    if (segment_size == 0) {
        printf("The heaphas size of 0!\n");
        breakpoint();
        return false;
    }
    size_t nfree = 0;
    size_t sum = 0;
    size_t *cur = segment_start;
    for (size_t i = 0; i < nheaders; i++) {
        sum = sum + HEADER_SIZE + ((*cur) & for_61_mask);
        if (((*cur) & for_3_mask) == 0) {
            nfree += 1;
        }
        if (i == nheaders - 1) {
            break;
        }
        cur = cur + ((*cur) & for_61_mask)/HEADER_SIZE + 1;
    }
    if (sum != segment_size || nfree != nfreeheaders) {
        breakpoint();
        return false;
    }
    
    cur = ffh;
    for (size_t i = 0; i < nfreeheaders; i++) {
        if (i == nfreeheaders - 1) {
            return (*(dll *)(cur + 1)).next == NULL;
        }
        cur = (*(dll *)(cur + 1)).next;
    }
    return true;
}

/* This function prints the header and the double linked list of the heap.
 */
void dump_heap() {
    printf("Heap segment starts at address %p, ends at %p, %lu headers currently used, %lu of them is free header.\n",
           segment_start, (char *)segment_start + segment_size, nheaders, nfreeheaders);

    size_t cur_header_dump = *segment_start;
    size_t *cur_position_dump = segment_start;
    size_t first_61_bits_dump = cur_header_dump & for_61_mask;
    size_t last_3_bits_dump = cur_header_dump & for_3_mask;
    dll cur_dpointer_dump;
    
    for (size_t i = 0; i < nheaders; i++) {
        if (last_3_bits_dump == 1) {
            strcpy(used_or_free, "used");
            printf("The %lu header: %lu bytes %s\n", i + 1, first_61_bits_dump, used_or_free);
        } else {
            strcpy(used_or_free, "free");
            printf("The %lu header: %lu bytes %s\n", i + 1, first_61_bits_dump, used_or_free);
            cur_dpointer_dump = *(dll *)(cur_position_dump + 1);
            if (cur_dpointer_dump.prev == NULL) {
                printf("The previous free block is: NULL.\n");
            } else {
                printf("The previous free block is: %p.\n", cur_dpointer_dump.prev);
            }
            if (cur_dpointer_dump.next == NULL) {
                printf("The next free block is: NULL.\n");
            } else {
                printf("The next free block is: %p.\n", cur_dpointer_dump.next);
            }
        }
        if (i == nheaders - 1) {
            break;
        }
        cur_position_dump = (size_t *)(cur_position_dump + (first_61_bits_dump + HEADER_SIZE)/HEADER_SIZE);
        cur_header_dump = *cur_position_dump;
        first_61_bits_dump = cur_header_dump & for_61_mask;
        last_3_bits_dump = cur_header_dump & for_3_mask;
    }
}
