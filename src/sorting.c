/*
 * Perform an in-place insertion sort of the array provided
 */
void insertion_sort(int **array, int size) {
    int *list = *array;

    for (int i=1; i<size; i++) {
        int item = list[i];
        int j = i - 1;

        while (j >= 0 && list[j] > item) {
            list[j + 1] = list[j];
            j--;
        }
        list[j + 1] = item;
    }
}
