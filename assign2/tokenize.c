#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "samples/prototypes.h"

#define FIXED_TEST_WORD_SIZE 100
#define FIXED_TEST_SYLLABLE_SIZE 10

// What cases does this test? Trace the code and find out!
static int fixed_test(void) {
    const char *input = "hip-po-pot-a-mus gir-affe e-le-phant kang-a-roo rhi-no-ce-ros";
    char word[FIXED_TEST_WORD_SIZE];
    char sub[FIXED_TEST_SYLLABLE_SIZE];

    while (scan_token(&input, " ", word, sizeof(word))) {
        int nsyllables = 0;
        const char *next = word;
        while (scan_token(&next, "-", sub, sizeof(sub))) {
            printf("%s", sub);
            nsyllables++;
        }
        int nletters = 0;
        next = word;
        while (scan_token(&next, "-", sub, 2)) {
            nletters++;
        }
        printf(" (%d syllables, %d letters)\n", nsyllables, nletters);
    }
    return 0;
}


int main(int argc, char *argv[]) {
    if (argc < 3) {
        return fixed_test();
    }

    const char *delimiters = argv[1];
    const char *input = argv[2];

    int bufsize = argc == 4 ? atoi(argv[3]) : strlen(input) + 1;
    char buf[bufsize];
    const char *remaining = input;
    printf("Tokenized: { ");
    while (scan_token(&remaining, delimiters, buf, sizeof(buf))) {
        printf("\"%s\" ", buf);
    }
    printf("}\n");

    // this should be just the null terminator
    printf("remaining: %s\n", remaining);

    return 0;
}
