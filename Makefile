# -*- Makefile -*-

SRC := $(wildcard src/**/mod.c src/util/**/mod.c)
OBJ := $(SRC:.c=.o)

TESTDIR := $(shell find src -name test.c -exec dirname {} \;)

SUBDIRS := $(wildcard src/*/. src/util/*/.)

# Make flags available to subdirs
export CFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -Wall -lGL -lglfw -lm -ldl -lblas

all: main $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

main: src/main.o $(SUBDIRS)
	gcc src/main.o src/glad.c $(OBJ) $(CFLAGS) -o main

test: $(TESTDIR)
$(TESTDIR):
	echo $@
	$(MAKE) test -C $@

.PHONY: test clean $(SUBDIRS) $(TESTDIR)
clean:
	rm -f src/main src/main.o

# Recurse into each subdir and execute make clean
	for dir in $(SUBDIRS); do \
		$(MAKE) clean -C $$dir; \
	done
