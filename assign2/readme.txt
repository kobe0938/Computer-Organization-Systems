File: readme.txt
Assignment: assign2
Author: Xiaokun Chen
Pronouns: He/him
----------------------

scan_token DOCUMENTATION
INSTRUCTIONS: Fill in the sections marked with a TODO below.
Your documentation should be original (i.e., please do not copy and paste from the assignment spec).
NAME
    scan_token - Scan_token is a function that tokenize a string by delimiters.
    bool scan_token(const char **p_input, const char *delimiters,
                    char buf[], size_t buflen);
ARGUMENTS
    const char **p_input - p_input is a pointer points the source string.(actually a pointer points
    to a pointer which points to the fisrt char of source string)
    const char *delimiters - delimiter is a string of seperators.(actually a pointer points to the
    fisrt char of the string)
    char buf[] - buf is a array that stores the string seperated by the delimiters.
    size_t buflen - buflen is the length of the buf array.
RETURN VALUE
    The return value would be true if there exists one string in the *input, in this case, if the
    buflen is larger than the next string length, it would store the next string until it meets a
    delimiter or a null terminator.
    If the buflen is smaller than or equal to the string length, it would return true as well, but
    the buf would store the first bquflen-1 item of the next string.
    The return value would be false, if other than the delimiters, the first char in the *input is
    a null terminator.
    Make sure to include a description of what will be stored in the buf argument upon return.
ASSUMPTIONS
    The scan_token function assumes that the buf argument has space for buflen characters.
    The scan_token function assumes that the buflen is greater than 1.
    The scan_token function assumes that the p_input is a valid pointer points to the source string.
    The scan_token function assumes that the *p_input does has a null terminator and could be a empty
    string.
    The scan_token function assumes that the delimiters does has a null terminator and contains one
    or more delimiters.
    
    Here is an example: The scan_token function assumes that the buf argument
    has space for buflen characters.
DESCRIPTION
    The header takes in four parameters as described in ARGUMENTS and return a bool. Then the function
    uses strspn to decide whether the first char is one of the delimiter. If it is, move the pointer
    *p_input to the next char and iterate this process until the current one is not a delimiter.
    Then judge whether the current char is the null terminator, return false if it is.
    After that, we know that there exist one string. So string_length gets the length of it until the
    next null terminator. If the string_length is less than the buflen, which means that this string
    can be fully put inside the buf[], update the pointer position, and write the string into buf with
    the last postion a null terminator and return. Otherwise, the string_length is greater the buflen,
    which means it needs to be seperated to two or more strings. Write the first buflen-1 item into buf[],
    update the *p_input pointer and return true.
    
    This section should include (high-level) implementation details. You can use your function-header
    comment as a starting point for this section.
