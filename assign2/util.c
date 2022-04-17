#include <string.h>
#include "samples/prototypes.h"

/* FUNCTION: get_env_value
 * ------------------------
Take in the envp, which is a array of pointers(to strings), and the key, which is a 
pointer points to a string named key(left part of '=' sign). The program would check 
each entry in the envp to see if the key matches. If mataches, return the pointer which
points to the right side of '=' sign of the corrsponding key!
 */
const char *get_env_value(const char *envp[], const char *key) {
    for (int i = 0; envp[i] != NULL; i++) {
        int key_length = strlen(key);
        char *value = strstr(envp[i], key);
        if (value != NULL && envp[i][key_length] == '=') {
            return value + key_length + 1;
        }
    }
    return NULL;
}

/* FUNCTION: scan_token
 * ------------------------
Scan token is a function that tokenize a string by delimiters.
 */

bool scan_token(const char **p_input, const char *delimiters, char buf[], size_t buflen) {
    while (strspn(*p_input, delimiters) != 0) {
        *p_input += 1;
    }
    if (**p_input == '\0') {
        return false;
    }
    int string_length = strcspn(*p_input, delimiters);
    if (string_length < buflen) {
        strncpy(buf, *p_input, string_length);
        buf[string_length] = '\0';
        *p_input += string_length;
        // now points to the delimiter or the '\0'
        return true;
    }
    // otherwise, we have to consider the buf length limitation below
    strncpy(buf, *p_input, buflen - 1);
    buf[buflen] = '\0';
    *p_input += (buflen - 1);           
    return true;
}
