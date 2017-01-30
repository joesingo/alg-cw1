#include <stdio.h>

const static char *PROGRAM_NAME = "sorting_test";

/*
 * Print usage text to stdout
 */
void print_usage() {
    printf("Usage: %s (insertion | counting) (best | worst | average) " \
           "<start size> <step> <no. of data points>\n", PROGRAM_NAME);
}

/*
 * Print an error message on stderr
 */
void print_error(char *message) {
    fprintf(stderr, "%s: %s\n", PROGRAM_NAME, message);
}
