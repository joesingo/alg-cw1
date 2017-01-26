#include <stdlib.h>

#include "sorting_test.h"

/*
 * Perform an in-place insertion sort of the array provided
 */
void insertion_sort(int *list, int size) {
    for (int i=1; i<size; i++) {
        int item = list[i];
        int j = i - 1;

        // Step backwards through the list starting from index i - 1 until we
        // reach an element smaller than list[i]
        while (j >= 0 && list[j] > item) {

            // Shift this item up a position in the list, since it is larger
            // than the item we are looking at
            list[j + 1] = list[j];
            j--;
        }

        // list[j] is smaller than item and everything larger than item has
        // been shifted along one position - therefore item should go at index
        // j+1
        list[j + 1] = item;
    }
}

/*
 * Perform counting sort on list and put the result in output
 */
void counting_sort(int *list, int *output, int size, int max_value) {
    int *count_array = create_array(max_value + 1);

    // Initialise count array to 0s
    for (int i=0; i<=max_value; i++) {
        count_array[i] = 0;
    }

    // Count the number of times each integer in [0, max_value] appears in list
    for (int j=0; j<size; j++) {
        count_array[list[j]]++;
    }

    // Compute the cumulative sum
    for (int i=1; i<=max_value; i++) {
        count_array[i] += count_array[i - 1];
    }

    // Put the elements in list into the correct position in output
    for (int j=size - 1; j>=0; j--) {
        output[count_array[list[j]] - 1] = list[j];

        // Decrease the count to handle the case where the current item appears
        // more than once in the list
        count_array[list[j]]--;
    }

    free(count_array);
}
