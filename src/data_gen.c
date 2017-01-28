#include <stdlib.h>

#include "data_gen.h"
#include "sorting.h"

/*
 * Return a random integer in the range [min, max]
 */
int random_number(int min, int max) {
    float r = (float) random() / RAND_MAX;  // r will be a rand float in [0, 1)
    return (int) (min + (max - min + 1) * r);
}


/*
 * Reverse an array of ints in place
 */
void reverse_array(int *list, int size) {
    for (int i=0; i<size / 2; i++) {
        int j = size - 1 - i;
        int temp = list[i];
        list[i] = list[j];
        list[j] = temp;
    }
}

/*
 * Comparison function for qsort() to sort integers in ascending order
 */
int int_comparison_asc(const void *a_p, const void *b_p) {
    const int *a_ptr = a_p;
    const int *b_ptr = b_p;
    return *a_ptr - *b_ptr;
}

/*
 * Comparison function for qsort() to sort integers in descending order
 */
int int_comparison_desc(const void *a_p, const void *b_p) {
    const int *a_ptr = a_p;
    const int *b_ptr = b_p;
    return *b_ptr - *a_ptr;
}

/*
 * Populate the destination array with random numbers according to the
 * parameters provided
 */
void generate_data(int *dest, DataGeneratorParams params, int size) {

    // Create random numbers between the min and max values provided
    for (int i=0; i<size; i++) {
        dest[i] = random_number(params.min, params.max);
    }

    if (params.ordering == SORTED || params.ordering == REVERSE_SORTED) {

        // Pointer to the comparison function to use
        int (*comparison_func)(const void *, const void *);

        if (params.ordering == SORTED) {
            comparison_func = &int_comparison_asc;
        }
        else {
            comparison_func = &int_comparison_desc;
        }

        // Ironically I do not use insertion/counting sort here since qsort
        // is much quicker!
        qsort(dest, size, sizeof(int), comparison_func);
    }
}
