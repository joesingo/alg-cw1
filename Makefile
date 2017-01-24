files = data_gen.c sorting_test.c sorting.c
default: $(files)
	gcc -o sorting_test $(files)