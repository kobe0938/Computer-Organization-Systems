/*
This file operates similarly to standard sort. It takes in one
directory and output the sorted entries inside this directory
line by line. It has four options: If invoked with the -l flag, 
the sort order is by increasing line length. If invoked with the -n flag,
the sort order is by string numerical value. If invoked with the -r flag, 
the sort order for mysort is reversed. If invoked with the -u flag, 
mysort discards duplicate lines. 
*/
#include <assert.h>
#include <errno.h>
#include <error.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "samples/prototypes.h"

#define MAX_LINE_LEN 4096
#define MIN_NLINES 100

typedef int (*cmp_fn_t)(const void *p, const void *q);

/* This function compares two strings using strcmp.
 */
int cmp_pstr(const void *p, const void *q) {
    return strcmp(*(char **)p, *(char **)q);
}
/* This function compares the length of two strings.
 */
int cmp_pstr_len(const void *p, const void *q) {
    return strlen(*(char **)p) - strlen(*(char**)q);
}

/* This function compares the numerical value of two strings.
 */
int cmp_pstr_numeric(const void *p, const void *q) {
    return atoi(*(char **)p) - atoi(*(char **)q);
}

/* This function is used to help print each element in the
print array.
 */
void print_helper(bool reverse, char **print_array, size_t size_used) {
    if (!reverse) {
        for (int i = 0; i < size_used; i++) {
            printf("%s", print_array[i]);
        }
    } else {
        for (int i = size_used - 1; i > -1; i--) {
            printf("%s", print_array[i]);
        }
    }
}

/* This function intends to free the memeory of every element of 
the free_array, and also free the free_array itself.
 */
void free_helper(char **free_array, size_t total_size) {
    for (size_t i = 0; i < total_size; i++) {
        free(free_array[i]);
    }
    free(free_array);
}

/* This function first read each line in the file. Sort and print them
according to the options(ie. -l -n -r -u introduced previously)  provided.
 */
void sort_lines(FILE *fp, cmp_fn_t cmp, bool uniq, bool reverse) {
    char buf[MAX_LINE_LEN];
    char **str_array = malloc(MIN_NLINES * sizeof(char *));
    assert(str_array != NULL);
    // size_used is to denote the pointers we used, size_limit is the total size we have
    size_t size_used = 0;
    size_t size_limit = MIN_NLINES;
    while (fgets(buf, MAX_LINE_LEN, fp) != NULL) {
        if ((size_used) == (size_limit - 1)) {
            size_limit *= 2;
            str_array = realloc(str_array, size_limit * sizeof(char *));
            assert(str_array != NULL);
        }
        str_array[size_used] = strdup(buf);
        size_used++;
    }
    if (uniq) {
        /*  If we use the unique option, create another heap array to binsert everything.
        The reason i choose heap (though it seems more propor to use stack because of the fixed
        length of the array) is that in the document it states stack may not large enough for this.
        Quote here :"Such a large array of lines could much too big for the stack."
        */
        char **uniq_str_array = malloc(size_used * sizeof(char*));
        assert(uniq_str_array != NULL);                         
        size_t uniq_size_used = 0;
        for (size_t i = 0; i < size_used; i++) {
            binsert(&str_array[i], uniq_str_array, &uniq_size_used, sizeof(char*), cmp);
        }
        // in order to avoid repeated code, I introduce two helper function
        print_helper(reverse, uniq_str_array, uniq_size_used);
        free_helper(str_array, size_used);
        free(uniq_str_array);
    } else {
        qsort(str_array, size_used, sizeof(char *), cmp);
        print_helper(reverse, str_array, size_used);
        free_helper(str_array, size_used);
    }
}

// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------
/*
This function operates similarly to standard sort. It takes in one
directory and output the sorted entries inside this directory
line by line. It has four options: If invoked with the -l flag, 
the sort order is by increasing line length. If invoked with the -n flag,
the sort order is by string numerical value. If invoked with the -r flag, 
the sort order for mysort is reversed. If invoked with the -u flag, 
mysort discards duplicate lines. 
*/
int main(int argc, char *argv[]) {
    cmp_fn_t cmp = cmp_pstr;
    bool uniq = false;
    bool reverse = false;

    int opt = getopt(argc, argv, "lnru");
    while (opt != -1) {
        if (opt == 'l') {
            cmp = cmp_pstr_len;
        } else if (opt == 'n') {
            cmp = cmp_pstr_numeric;
        } else if (opt == 'r') {
            reverse = true;
        } else if (opt == 'u') {
            uniq = true;
        } else {
            // this is to deal with the error case
            return 1;
        }
        // this is to get the opt next round
        opt = getopt(argc, argv, "lnru");
    }

    FILE *fp = stdin;
    if (optind < argc) {
        fp = fopen(argv[optind], "r");
        if (fp == NULL) {
            error(1, 0, "cannot access %s", argv[optind]);
        }
    }
    // calls sort_lines after preparation
    sort_lines(fp, cmp, uniq, reverse);
    fclose(fp);
    return 0;
}
