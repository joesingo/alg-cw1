#include <stdio.h>

const static char *PROGRAM_NAME = "sorting_test";

/*
 * Print usage text to stdout
 */
void print_usage() {
    printf("Usage: %s (insertion | counting) (best | worst | average) " \
           "<min size> <max size> <no. of data points> [-q]\n", PROGRAM_NAME);
}

void print_help() {
    print_usage();
    printf("\nOptions:\n");
    printf("  -h    Show this help\n");
    printf("  -q    Arrange data points so that their squares are equally\n" \
           "        spaced apart. By default the data points themselves are\n" \
           "        spaced apart equally\n");
}

/*
 * Print an error message on stderr
 */
void print_error(char *message) {
    fprintf(stderr, "%s: %s\n", PROGRAM_NAME, message);
}
