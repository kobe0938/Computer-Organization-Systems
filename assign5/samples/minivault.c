#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>

int stage1(char *str) {
    char *occurrence = strstr(str, "cs107");
    if (occurrence == NULL) return 0;
    return 1;
}

int stage2(char *str) {
    int num = atoi(str);
    if (num > 10) return 0;
    if (num % 2 == 0) return 0;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc <= 2) {
        error(1, 0, "Not enough arguments.  ./minivault [password1] [password2]");
    }

    if (stage1(argv[1])) {
        printf("Stage 1 passed!\n");
        if (stage2(argv[2])) {
            printf("Stage 2 passed!\nYou cracked the minivault!\n");
            return 0;
        }
    }

    printf("The minivault alarm triggered!\n");
    return 1;
}
