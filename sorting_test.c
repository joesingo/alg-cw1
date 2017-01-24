#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


/*
 * An enum to represent the different ways a list of randomly generated numbers
 * can be ordered
 */
typedef enum {
    SORTED,
    REVERSE_SORTED,
    RANDOM
} Ordering;


/*
 * A struct to hold parameters to be passed to generate_data() below
 */
typedef struct {
    int min;
    int max;
    Ordering ordering;
} DataGeneratorParams;


/*
 * Return a random number in the range [min, max]
 */
float random_number(int min, int max) {
    float r = (float) random() / RAND_MAX;  // r will be a rand float in [0, 1]
    return min + (max - min) * r;
}


/*
 * Reverse an array of floats in place
 */
void reverse_array(float **array, int size) {
    float *list = *array;

    for (int i=0; i<size / 2; i++) {
        int j = size - 1 - i;
        float temp = list[i];
        list[i] = list[j];
        list[j] = temp;
    }
}


/*
 * Perform an in-place sort of the array provided
 */
void insertion_sort(float **array, int size) {
    float *list = *array;

    for (int i=1; i<size; i++) {
        float item = list[i];
        int j = i - 1;

        while (j >= 0 && list[j] > item) {
            list[j + 1] = list[j];
            j--;
        }
        list[j + 1] = item;
    }
}


/*
 * Return a pointer to an array of random numbers according to the parameters
 * provided
 */
float *generate_data(DataGeneratorParams params, int size) {
    float *list = malloc(size * sizeof(float));

    // Create random number between the min and max values provided
    for (int i=0; i<size; i++) {
        list[i] = random_number(params.min, params.max);
    }

    if (params.ordering == SORTED || params.ordering == REVERSE_SORTED) {

        // If the data is to be (reverse) sorted then use insertion sort
        insertion_sort(&list, size);

        if (params.ordering == REVERSE_SORTED) {
            reverse_array(&list, size);
        }
    }

    return list;
}


void main(int argc, char **argv) {

    srandom(time(NULL));

    int NO_OF_TESTS = 30;
    int TEST_REPETITIONS = 3;

    int default_min = 1;
    int default_max = 100;

    DataGeneratorParams insertion_best_case;
    insertion_best_case.min = default_min;
    insertion_best_case.max = default_max;
    insertion_best_case.ordering = SORTED;

    DataGeneratorParams insertion_worst_case;
    insertion_worst_case.min = default_min;
    insertion_worst_case.max = default_max;
    insertion_worst_case.ordering = REVERSE_SORTED;

    DataGeneratorParams insertion_avg_case;
    insertion_avg_case.min = default_min;
    insertion_avg_case.max = default_max;
    insertion_avg_case.ordering = RANDOM;

    int start_size = 1000;
    int step = 500;
    int num_sizes = 30;

    int sizes[num_sizes];
    for (int i=0; i<num_sizes; i++) {
        sizes[i] = start_size + i * step;
    }

    for (int i=0; i<num_sizes; i++) {

        double total_time = 0;

        for (int j=0; j<NO_OF_TESTS; j++) {

            float *list = generate_data(insertion_best_case, sizes[i]);

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
