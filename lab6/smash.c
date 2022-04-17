/* CS107, Lab 6
 * -------------
 * In the code below, it seems like it isn't possible for greet to return anything
 * other than 2.  Or is it....?  Your task - design an input that makes greet
 * return 107.  Use your assembly and diagramming skills!
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_LEN 16

int greet() {
    int num = 2;
    char buf[BUF_LEN];

    printf("What is your name? ");
    if (gets(buf)) {
        printf("Hello, %s! Buffer has space for %zu chars, your name is length %zu.\n",
            buf, sizeof(buf), strlen(buf));
        if (strlen(buf) > sizeof(buf)) {
            printf("You have overrun the buffer by %zu chars.\n",
                strlen(buf) - sizeof(buf) + 1);
        }
    }

    return num;
}

int main(int argc, char *argv[]) {
    printf("You've got 5 tries to blow this popsicle stand. Go!\n");

    for (int i = 0; i < 5; i++) {
        int result = greet();
        printf("greet() returned %d\n", result);
        if (result == 107) {
            printf("You did it??  But how??\n");
        }
    }
    return 0;
}
