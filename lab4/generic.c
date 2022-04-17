/*
 * CS107
 * Lab 4
 *
 * This file shows examples of using comparison functions when
 * sorting.  See the lab writeup for more information.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int compare_strings_alphabetically(const void *a, const void *b) {
    const char *first  = *(const char **)a;
    const char *second = *(const char **)b;

    return strcmp(first, second);

    // could also combine all lines as
    // return strcmp(*(const char **)a, *(const char **)b);
}

int compare_strings_by_descending_length(const void *a, const void *b) {
    const char *first = *(const char **)a;
    const char *second = *(const char **)b;

    return strlen(second) - strlen(first);

    // could also combine all lines as
    // return strlen(*(const char **)b) - strlen(*(const char **)a);
}

int compare_strings_by_first_character(const void *a, const void *b) {
    // TODO: remove this line and put your code here!
    return 0;
}

void sort_city_names() {
    char *city_names[] = {"Stanford", "Seattle", "Stevinson", "Redwood City", "Los Angeles"};
    int ncities = sizeof(city_names) / sizeof(city_names[0]);

    // Alphabetically
    printf("Sort city names alphabetically: \n");
    qsort(city_names, ncities, sizeof(city_names[0]), compare_strings_alphabetically);
    for (int i = 0; i < ncities; i++) {
        printf("%s", city_names[i]);
        if (i < ncities - 1) {
            printf(", ");
        }
    }
    printf("\n");

    // Longest to shortest
    printf("Sort city names by decreasing length: \n");
    qsort(city_names, ncities, sizeof(city_names[0]), compare_strings_by_descending_length);
    for (int i = 0; i < ncities; i++) {
        printf("%s", city_names[i]);
        if (i < ncities - 1) {
            printf(", ");
        }
    }

    printf("\n");

    // By first character
    printf("Sort city names alphabetically by first character: \n");
    qsort(city_names, ncities, sizeof(city_names[0]), compare_strings_by_first_character);
    for (int i = 0; i < ncities; i++) {
        printf("%s", city_names[i]);
        if (i < ncities - 1) {
            printf(", ");
        }
    }

    printf("\n\n");
}

int compare_letters_alphabetically(const void *a, const void *b) {
    unsigned char first  = *(const char *)a;
    unsigned char second = *(const char *)b;

    return first - second;

    // could also combine all lines as
    // return *(const char *)a - *(const char *)b;
}

int compare_integers_ascending(const void *a, const void *b) {
    int first = *(const int *)a;
    int second = *(const int *)b;

    return first - second;

    // could also combine all lines as
    // return *(const int *)a - *(const int *)b;
}

void sort_integers() {
    int nums[] = {0x33444433, 0x59111159, 0x67, 0};
    int n = sizeof(nums) / sizeof(nums[0]);

    printf("correct call qsort = ");
    qsort(nums, n, sizeof(nums[0]), compare_integers_ascending);
    for (int i = 0; i < n; i++) {
        printf("%#x ", nums[i]);
    }
    printf("\n");

    printf("incorrect call qsort = ");
    qsort(nums, n, sizeof(nums[0]), compare_letters_alphabetically);
    for (int i = 0; i < n; i++) {
        printf("%#x ", nums[i]);
    }
    printf("\n");
}


int main(int argc, char *argv[]) {
    sort_city_names();
    sort_integers();
    return 0;
}
