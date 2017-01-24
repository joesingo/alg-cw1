#include <stdlib.h>

#include "data_gen.h"
#include "sorting.h"

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
