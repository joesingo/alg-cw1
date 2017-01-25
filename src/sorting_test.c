#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "sorting.h"
#include "data_gen.h"

int NO_OF_TESTS = 30;

int START_SIZE = 100;
int SIZE_STEP = 100;
int NUM_SIZES = 30;

char *PROGRAM_NAME;

/*
 * An enum to hold the different scenarios for performing the test
 */
typedef enum {
    BEST_CASE,
    WORST_CASE,
    AVERAGE_CASE
} Scenario;

/*
 * Print an error message on stderr
 */
void print_error(char *message) {
    fprintf(stderr, "%s: %s\n", PROGRAM_NAME, message);
}

/*
 * Print usage text to stdout
 */
void print_usage() {
    printf("Usage: %s (insertion | counting) (best | worst | average)\n",
           PROGRAM_NAME);
}

/*
 * Validate the provided command line arguments and set the scenario and
 * algorithm using the pointers provided
 */
void get_args(int argc, char **argv, Scenario *scenario, SortingAlgorithm *alg) {
    // Check the correct number of arguments have been provided
    if (argc < 3) {
        print_error("Invalid arguments");
        print_usage();
        exit(1);
    }

    // Check the scenario (best/worst/average) is valid
    if (strcmp(argv[2], "best") == 0) {
        *scenario = BEST_CASE;
    }
    else if (strcmp(argv[2], "worst") == 0) {
        *scenario = WORST_CASE;
    }
    else if (strcmp(argv[2], "average") == 0) {
        *scenario = AVERAGE_CASE;
    }
    else {
        print_error("Invalid scenario");
        print_usage();
        exit(1);
    }

    // Check sorting algorithm is valid
    if (strcmp(argv[1], "insertion") == 0) {
        *alg = INSERTION_SORT;
    }
    else if (strcmp(argv[1], "counting") == 0) {
        *alg = COUNTING_SORT;
    }
    else {
        print_error("Invalid sorting algorithm");
        print_usage();
        exit(1);
    }

}

void print_list(int *list, int size) {
    for (int i=0; i<size; i++) {
        printf("%d, ", list[i]);
    }
    printf("\n");
}

void main(int argc, char **argv) {
    srandom(time(NULL));
    PROGRAM_NAME = "sorting_test";

    Scenario scenario;
    SortingAlgorithm alg;
    get_args(argc, argv, &scenario, &alg);

    // Set data gen parameters to default values
    DataGeneratorParams params;
    params.min = 0;
    params.max = 100;
    params.ordering = RANDOM;

    // Set data gen params according to chosen algorithm
    switch (alg) {

        case INSERTION_SORT:
            if (scenario == BEST_CASE) {
                params.ordering = SORTED;
            }
            else if (scenario == WORST_CASE) {
                params.ordering = REVERSE_SORTED;
            }
            break;

        case COUNTING_SORT:
            if (scenario == AVERAGE_CASE) {
                print_error("Invalid scenario for counting sort - please choose " \
                            "'best' or 'worst'");
                exit(1);
            }
            break;
    }

    // Caclulate the array sizes to use based on the values defined above
    int sizes[NUM_SIZES];
    for (int i=0; i<NUM_SIZES; i++) {
        sizes[i] = START_SIZE + i * SIZE_STEP;
    }

    // Perform the actual test
    for (int i=0; i<NUM_SIZES; i++) {

        double total_time = 0;

        for (int j=0; j<NO_OF_TESTS; j++) {

            // If using counting sort then adjust the max value to be generated
            // according to best/worst case scenario
            if (alg == COUNTING_SORT) {

                if (scenario == BEST_CASE) {
                    // Good performance when k = n
                    params.max = sizes[i];
                }

                else if (scenario == WORST_CASE) {
                    // Bad performance is when k is significantly larger than n,
                    // e.g. n^2
                    params.max = sizes[i] * sizes[i];
                }
            }

            int list[sizes[i]];
            generate_data(list, params, sizes[i]);

            #ifdef DEBUG
            printf("List to sort is:\n");
            print_list(list, sizes[i]);
            #endif

            clock_t start;
            clock_t end;

            if (alg == INSERTION_SORT) {
                start = clock();
                insertion_sort(list, sizes[i]);
                end = clock();
            }
            else if (alg == COUNTING_SORT) {
                int output[sizes[i]];
                start = clock();
                counting_sort(list, output, sizes[i], params.max);
                end = clock();
            }

            total_time += (double)(end - start) / CLOCKS_PER_SEC;

            #ifdef DEBUG
            printf("Sorted list is:\n");
            print_list(list, sizes[i]);

            printf("--\n");
            #endif
        }

        double time_taken = total_time / NO_OF_TESTS;
        printf("%d,%f\n", sizes[i], time_taken);
    }
}
