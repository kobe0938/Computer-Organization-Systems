#include <assert.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "samples/prototypes.h"

#define MAX_NUM_LINES 100000
#define DEFAULT_NUM_LINES 10
#define NUMERIC_ARG_BASE 10
/* This file takes in the pointer points to a file and the number
of lines we want to display. Then output the last number of lines 
in the file.
*/


/* This function prints the last n lines in the file that 
file_pointer points to.
*/
void print_last_n(FILE *file_pointer, int n) {
    char *array_of_strings[n];
    for (int i = 0; i < n; i++) {
        array_of_strings[i] = NULL;
    }
    char *line = NULL;
    int index_number = 0;
    while ((line = read_line(file_pointer)) != NULL) {
        int mod = index_number % n;
        free(array_of_strings[mod]);
        array_of_strings[mod] = strdup(line);
        assert(array_of_strings[mod] != NULL);
        free(line);
        index_number++;
    }
    
    // intent to check if the file has at least n elements
    if (index_number < n) {
        n = index_number;
    }
    
    for (int i = index_number; i < n + index_number; i++) {
        printf("%s\n", array_of_strings[i % n]);
    }
    for (int i = 0; i < n; i++) {
        free(array_of_strings[i]);
    }
}

// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------
/* This function convert the arguments and raise error when nessesery.
 */
int convert_arg(const char *str, int max) {
    char *end = NULL;
    long parsed_number = strtol(str, &end, NUMERIC_ARG_BASE);
    if (*end != '\0') {
        error(1, 0, "Invalid number '%s'", str);
    }
    if (parsed_number < 1 || parsed_number > max) {
        error(1, 0, "%s is not within the acceptable range [%d, %d]", str, 1, max);
    }
    return parsed_number;
}

/* This function takes in the pointer points to a file and the number
of lines we want to display. Then output the last number of lines 
in the file.
*/
int main(int argc, char *argv[]) {
    int num_lines = DEFAULT_NUM_LINES;

    if (argc > 1 && argv[1][0] == '-') {
        num_lines = convert_arg(argv[1] + 1, MAX_NUM_LINES);
        argv++;
        argc--;
    }

    FILE *file_pointer = NULL;
    if (argc == 1) {
        file_pointer = stdin;
    } else {
        file_pointer = fopen(argv[1], "r");
        if (file_pointer == NULL) {
            error(1, errno, "cannot access '%s'", argv[1]);
        }
    }

    print_last_n(file_pointer, num_lines);
    fclose(file_pointer);
    return 0;
}
