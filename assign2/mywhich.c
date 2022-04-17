#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "samples/prototypes.h"

/* FUNCTION: main
 * ------------------------
The main function takes in one or more argv. 
If there is only one argv, print all the paths.
If there are more than one argv, print the path of each
argv from argv[1] if there is a readable and exacutable 
file within that matched path.
 */
int main(int argc, char *argv[], const char *envp[]) {
    const char *searchpath = get_env_value(envp, "MYPATH");
    if (searchpath == NULL) {
        searchpath = get_env_value(envp, "PATH");
    }
    
    if (argc == 1) {
        char dir[PATH_MAX];
        const char *remaining = searchpath;

        printf("Directories in search path:\n");
        while (scan_token(&remaining, ":", dir, sizeof(dir))) {
            printf("%s\n", dir);
        }
    } else {
        for (int i = 1; i < argc; i++) {
            char dir[PATH_MAX];
            const char *remaining = searchpath;
            while (scan_token(&remaining, ":", dir, sizeof(dir))) {
                char *dirptr1 = strcat(dir, "/");
                char *dirptr2 = strcat(dirptr1, argv[i]);
                if (access(dirptr2, R_OK | X_OK) == 0) {
                    printf("%s\n", dir);
                    break;
                }
            }
        }
    }
    return 0;
}
