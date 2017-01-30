#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "sorting.h"
#include "data_gen.h"
#include "error.h"

// The number of times to generate and sort data for each array size
#ifndef NO_OF_TESTS
// This is #define'd so that it may be overriden in debug build
#define NO_OF_TESTS 50
#endif

/*
 * An enum to hold the different scenarios for performing the test
 */
typedef enum {
    BEST_CASE,
    WORST_CASE,
    AVERAGE_CASE
} Scenario;

/*
 * Return a pointer to an array of ints of a given size, and show an error
 * message and quit if memory could not be allocated. REMEMBER TO FREE MEMORY
 * ONCE FINISHED WHEN USING THIS FUNCTION
 */
int *create_array(int size) {
    int *p = malloc(size * sizeof(int));

    if (size != 0 && p == NULL) {
        print_error("Failed to allocate memory");
        exit(1);
    }

    return p;
}

/*
 * Convert str to an int and exit with the error message provided if str is
 * invalid
 */
int read_pos_integer(char *str, char *e_message) {
    char *endptr;  // Points to first invalid character
    int i = strtol(str, &endptr, 10);

    if (endptr == str) {
        // If first character is invalid then we have no number, so error
        print_error(e_message);
        exit(1);
    }
    else if (i <= 0) {
        print_error(e_message);
        exit(1);
    }

    return i;
}

/*
 * Validate the provided command line arguments and set the scenario, algorithm,
 * start size, step size, and no. of data points
 */
void get_args(int argc, char **argv, Scenario *scenario, SortingAlgorithm *alg,
              int *start_size, int *step_size, int *num_sizes) {

    // Check the correct number of arguments have been provided
    if (argc < 6) {
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

    *start_size = read_pos_integer(argv[3], "Invalid start size");
    *step_size = read_pos_integer(argv[4], "Invalid step size");
    *num_sizes = read_pos_integer(argv[5], "Invalid number of sizes");
}

/*
 * Print an array of integers to stdout (for debugging/testing purposes)
 */
void print_list(int *list, int size) {
    for (int i=0; i<size; i++) {
        printf("%d", list[i]);

        if (i + 1 < size) {
            printf(", ");
        }
    }
    printf("\n");
}

/*
 * Generate a list according to the parameters provided and sort it using the
 * specified algorithm. Return the time taken in seconds
 */
double perform_test(DataGeneratorParams params, SortingAlgorithm alg, int size) {

    int *list = create_array(size);
    generate_data(list, params, size);

    #ifdef DEBUG
    printf("List to sort is:\n");
    print_list(list, size);
    #endif

    clock_t start;
    clock_t end;

    if (alg == INSERTION_SORT) {
        start = clock();
        insertion_sort(list, size);
        end = clock();
    }
    else if (alg == COUNTING_SORT) {
        int *output = create_array(size);

        start = clock();
        counting_sort(list, output, size, params.max);
        end = clock();

        free(output);
    }

    free(list);

    // clock() returns the processor time used so far, so divide the
    // difference between end and start by CLOCKS_PER_SEC to convert to
    // seconds
    return (double)(end - start) / CLOCKS_PER_SEC;
}

int main(int argc, char **argv) {
    srandom(time(NULL));

    Scenario scenario;
    SortingAlgorithm alg;
    int start_size, step_size, num_sizes;
    get_args(argc, argv, &scenario, &alg, &start_size, &step_size, &num_sizes);

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

    // Perform the actual test
    for (int i=0; i<num_sizes; i++) {

        int size = start_size + i * step_size;
        double total_time = 0;

        // For each array size perform the test NO_OF_TESTS times, then
        // calculate the average time taken
        for (int j=0; j<NO_OF_TESTS; j++) {

            // If using counting sort then adjust the max value to be generated
            // according to best/worst case scenario
            if (alg == COUNTING_SORT) {

                if (scenario == BEST_CASE) {
                    // Good performance when k = n
                    params.max = size;
                }

                else if (scenario == WORST_CASE) {
                    // Bad performance is when k is significantly larger than n,
                    // e.g. n^2. Note that we could also choose n^3, 2^n, n!
                    // etc to get even worse performance
                    params.max = size * size;
                }
            }

            total_time += perform_test(params, alg, size);
        }

        // Print out the array size and average time taken in CSV format
        double time_taken = total_time / NO_OF_TESTS;
        printf("%d,%f\n", size, time_taken);
    }

    return 0;
}
