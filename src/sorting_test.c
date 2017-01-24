#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sorting.h"
#include "data_gen.h"

int NO_OF_TESTS = 30;
int TEST_REPETITIONS = 3;

int START_SIZE = 100;
int SIZE_STEP = 100;
int NUM_SIZES = 30;

void main(int argc, char **argv) {
    srandom(time(NULL));

    // Set data gen parameters to default values
    DataGeneratorParams params;
    params.min = 1;
    params.max = 100;
    params.ordering = RANDOM;

    // TODO: Work out parameters based on sorting algorithm and scenario from
    // command line args

    // Caclulate the array sizes to use based on the values defined above
    int sizes[NUM_SIZES];
    for (int i=0; i<NUM_SIZES; i++) {
        sizes[i] = START_SIZE + i * SIZE_STEP;
    }

    for (int i=0; i<NUM_SIZES; i++) {

        double total_time = 0;

        for (int j=0; j<NO_OF_TESTS; j++) {

            float *list = generate_data(params, sizes[i]);

            for (int k=0; k<TEST_REPETITIONS; k++) {
                clock_t start = clock();
                insertion_sort(&list, sizes[i]);
                clock_t end = clock();
                total_time += (double)(end - start) / CLOCKS_PER_SEC;
            }

            free(list);
        }

        double time_taken = total_time / (NO_OF_TESTS * TEST_REPETITIONS);
        printf("%d,%f\n", sizes[i], time_taken);
    }
}
