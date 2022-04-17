#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "samples/prototypes.h"

/* This file intends to takes in one pointer pointes to a file and read
out(output) each line of that file until it meats the EOF. Each time, it 
reads out one line(until the newline symbol, if the buf size is not enough
, then double it.
*/

/* initial allocation will be for min size, if not big enough, doubles
 * to 64, then 128, then 256, etc. as needed to accommodate the entire line
 * resize-as-you-go, doubling each time.
 */
#define MINIMUM_SIZE 32
const int CUMULATIVE_INT = 1;


char *read_line(FILE *file_pointer) {
    int old_size = MINIMUM_SIZE;
    int curr_size = old_size;
    int cumulative_factor = CUMULATIVE_INT;
    char *buf = malloc(sizeof(char) * old_size);
    assert(buf != NULL);
    char * next_line = fgets(buf, MINIMUM_SIZE, file_pointer);
    if (next_line == NULL) {
        free(buf);
        return NULL;
    }
    
    while (strlen(buf) == (old_size - cumulative_factor)) {
        if (buf[old_size - cumulative_factor - 1] == '\n') {
            break;
        }
        curr_size = old_size * 2;
        char *str = malloc(sizeof(char) * old_size);
        assert(str != NULL);

        buf = realloc(buf, sizeof(char) * curr_size);
        assert(buf != NULL);
        next_line = fgets(str, old_size, file_pointer);

        strcat(buf, str);
        old_size = curr_size;
        cumulative_factor += 1;
        free(str);
    }
    if (buf[strlen(buf) - 1] == '\n') {
        buf[strlen(buf) - 1] = '\0';
    }
    return buf;
}
