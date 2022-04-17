/* This file takes in the option(-a and -z) to decide to print all the
entries in the directory including hidden ones or not, and to print the 
entries by type order or by name order. This function can take several
varibles as dirctories after the option and print every entry in those 
directories. Please see the order details other details in the function.
 */
#include <dirent.h>
#include <error.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { SORT_BY_NAME, SORT_BY_TYPE };
enum { EXCLUDE_DOT, INCLUDE_DOT };

/* This fully implemented function returns whether the dirent pointed to by
 * the given pointer represents a directory.  (Note: on the myth filesystem,
 * the only file type information that is accurate is directory/not-directory
 * used here. Other type info in struct dirent is not reliable).
 */
bool is_dir(const struct dirent *dirptr) {
    return dirptr->d_type == DT_DIR;
}
/* This function is to order the dir1's d_name and dir2's d_name by alphabetical
order.
*/
int string_order(const struct dirent **dir1, const struct dirent **dir2) {
    return strcmp((*dir1)->d_name, (*dir2)->d_name);
}

/* This function is to order the dir1's d_name and dir2's d_name by type
order. And if one of them is the directory and the other is a file, always
put the directory first. If they are the same type, order by alphabetical
order.
*/
int type_order(const struct dirent **dir1, const struct dirent **dir2) {
    int return_val = 0;
    // the strategy is judge whether dir1 and dir2 are same type or not
    if (is_dir(*dir1) && !is_dir(*dir2)) {
        return_val = -1;
    } else if (!is_dir(*dir1) && is_dir(*dir2)) {
        return_val = 1;
    } else {
        return_val = string_order(dir1, dir2);
    }
    return return_val;
}

/* This function judge whether the first char of a file or directory is a
'.', which means it is a hidden file or hidden directory.
*/ 
int dot_filter(const struct dirent* dir1) {
    if (*(dir1 -> d_name) == '.') {
        return 0;
    }
    return 1;
}

/* This function takes in the directory path, and two function pointer to 
denote which filter function and compare function we should use. Then use 
scandir to get every enttries in the path. Then print them each.
*/
void ls(const char *dirpath, int filter, int order) {
    // here I choose to use the typedef so that I need to call only once scandir on line 90
    typedef int (*cmp_fn_t1)(const struct dirent **, const struct dirent **);
    cmp_fn_t1 order_func = NULL;
    typedef int (*cmp_fn_t2)(const struct dirent *);
    cmp_fn_t2 filter_func = NULL;

    // we change the function pointer according to the requirement
    if (filter == EXCLUDE_DOT) {
        filter_func = dot_filter;
        if (order == SORT_BY_NAME) {
            order_func = string_order;
        } else {
            order_func = type_order;
        }
    } else if (filter == INCLUDE_DOT) {
        filter_func = NULL;
        if (order == SORT_BY_NAME) {
            order_func = string_order;
        } else {
            order_func = type_order;
        }
    } 

    struct dirent **namelist = NULL;
    int num_entries = scandir(dirpath, &namelist, filter_func, order_func);
    if (num_entries == -1) {
        // this error will not stop the program
        error(0, 0, "cannot access %s", dirpath);
        // however, we need to skip 'this round' with return
        return;
    }
    for (int i = 0; i < num_entries; i++) {
        // judge whether we need to print an extra '/'
        if (is_dir(namelist[i])) {
            printf("%s/\n", namelist[i] -> d_name);
        } else {
            printf("%s\n", namelist[i] -> d_name);
        }
        free(namelist[i]);
    }
    free(namelist);
}

// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------
/* This function takes in the option(-a and -z) to decide to print all the
entries in the directory including hidden ones or not, and to print the 
entries by type order or by name order. This function can take several
varibles as dirctories after the option and print every entry in those 
directories. Please see the order details other details in the function.
 */
int main(int argc, char *argv[]) {
    int order = SORT_BY_NAME;
    int filter = EXCLUDE_DOT;

    int opt = getopt(argc, argv, "az");
    while (opt != -1) {
        if (opt == 'a') {
            filter = INCLUDE_DOT;
        } else if (opt == 'z') {
            order = SORT_BY_TYPE;
        } else {
            return 1;
        }

        opt = getopt(argc, argv, "az");
    }
    
    if (optind < argc - 1) {
        for (int i = optind; i < argc; i++) {
            printf("%s:\n", argv[i]);
            ls(argv[i], filter, order);
            printf("\n");
        }
    } else {
        ls(optind == argc - 1 ? argv[optind] : ".", filter, order);
    }
    
    return 0;
}
