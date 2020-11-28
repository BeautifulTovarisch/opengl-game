# -*- Makefile -*-

mod.o: mod.c mod.h
	gcc -c mod.c -o mod.o

test: mod.o
	gcc test.c mod.o $(CFLAGS) -l cmocka -o test
	./test
	rm -f test

.PHONY: clean test

clean:
	rm -f mod mod.o
