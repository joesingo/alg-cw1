files = src/data_gen.c src/sorting_test.c src/sorting.c src/error.c
exec_name = sorting_test
default: $(files)
	gcc -o $(exec_name) $(files) -lm

debug: $(files)
	gcc -o $(exec_name) -D DEBUG -D NO_OF_TESTS=1 $(files) -lm