files = src/data_gen.c src/sorting_test.c src/sorting.c
default: $(files)
	gcc -o sorting_test $(files)