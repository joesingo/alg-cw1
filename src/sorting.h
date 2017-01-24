#ifndef SORTING_H
#define SORTING_H

typedef enum {
    INSERTION_SORT,
    COUNTING_SORT
} SortingAlgorithm;

void insertion_sort(int **array, int size);

#endif