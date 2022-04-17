#include <assert.h>
#include <errno.h>
#include <error.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "samples/prototypes.h"

/* This file reads in a file, and count how many times each line show up
repeatedily, in the end, this program would print the summation time of each 
line show up in the file once, according to the first appear order.
*/

/* initial estimate of number of uniq lines
 * resize-as-you-go, add in increments of 100
 */
#define ESTIMATE 100

void print_and_free_helper(int size, void *each_line_v, int curr_number) {
      typedef struct {
        char *txt;
        int count;
    } EACH_LINE;
    EACH_LINE *each_line = (EACH_LINE *)each_line_v;
    for (int i = 0; i < size; i++) {
        if (each_line[i].count == 0) {
            break;
        }
        printf("%7d  %s\n", each_line[i].count, each_line[i].txt);
    }
    
    for (int i = 0; i < curr_number; i++) {
        free(each_line[i].txt);
    }
    free(each_line);
}

/* This function takes in a pointer and output each unique line in it by
order. Besides, it counts the appearing time each same line appears.
*/
void print_uniq_lines(FILE *file_pointer) {
    int size = ESTIMATE;
    typedef struct {
        char *txt;
        int count;
    } EACH_LINE;
    EACH_LINE *each_line = malloc(size * sizeof(EACH_LINE));
    assert(each_line != NULL);

    for (int i = 0; i < size; i++) {
        each_line[i].count = 0;
    }
    char *line = NULL;
    int curr_number = 0;
    bool flag = false;
    while ((line = read_line(file_pointer)) != NULL) {
        if (curr_number >= size) {
            EACH_LINE *backup_txt = malloc(sizeof(EACH_LINE) * size);
            assert(backup_txt != NULL);          
            memcpy(backup_txt, each_line, size * sizeof(EACH_LINE));        
            size += ESTIMATE;
            each_line = realloc(each_line, size * sizeof(EACH_LINE));
            assert(each_line != NULL);
            memcpy(each_line, backup_txt, (size - ESTIMATE) * sizeof(EACH_LINE));
            free(backup_txt); 
            for (int i = size - ESTIMATE; i < size; i++) {
                each_line[i].count = 0;
            } 
        } 
        // find exsiting string, count plus one
        for (int i = 0; i < curr_number; i++) {
            if (strcmp(each_line[i].txt, line) == 0) {
                each_line[i].count += 1;
                flag = true;
            }
        }     
        // otehrwise, document the new string
        if (!flag) {
            each_line[curr_number].txt = strdup(line);
            each_line[curr_number].count++;
            curr_number++;
        }
        flag = false;
        free(line);
    }
    print_and_free_helper(size, each_line, curr_number);
}
    
// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------
/* This file reads in a file, and count how many times each line show up
repeatedily, in the end, this program would print the summation time of each 
line show up in the file once, according to the first appear order.
*/
int main(int argc, char *argv[]) {
    FILE *file_pointer = NULL;

    if (argc == 1) {
        file_pointer = stdin;
    } else {
        file_pointer = fopen(argv[1], "r");
        if (file_pointer == NULL) {
            error(1, errno, "cannot access '%s'", argv[1]);
        }
    }

    print_uniq_lines(file_pointer);
    fclose(file_pointer);
    return 0;
}
