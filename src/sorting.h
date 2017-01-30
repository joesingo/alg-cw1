#ifndef SORTING_H
#define SORTING_H

typedef enum {
    INSERTION_SORT,
    COUNTING_SORT
} SortingAlgorithm;

void insertion_sort(int *list, int size);
void counting_sort(int *list, int *output, int size, int max_value);

#endif
