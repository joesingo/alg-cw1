#ifndef DATA_GEN_H
#define DATA_GEN_H

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

int *generate_data(DataGeneratorParams params, int size);

#endif