# Some notes
# $@ is the left side of :
# $^ is the right side of :
# .PHONY rule keeps make from doing something with a file named clean, help, etc.


MAKEFLAGS += -rR # eliminates built-in implicit rules
CC = gcc
CFLAGS  = -Wall -g
INCLUDES = -I./lib/libgit2/include/ -I./lib/libgit2/src/
LDFLAGS = -L./lib/libgit2/build/
LIBRARIES = -lgit2

ODIR = src
_OBJ = git-release.o tags.o memory.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


#
# PHONY target below
#

all: bin/git-release

clean: 
	rm -f src/*.o
	rm -f bin/*

clean-lib:
	rm -rf lib/semver-utils/out lib/libgit2/build

help:
	@echo "help below"

#
# Files below
#

lib/libgit2/build/:
	git submodule update --init --recursive
	mkdir -p lib/libgit2/build
	cd lib/libgit2/build && cmake .. && cmake --build .
lib/semver-utils/out/:
	git submodule update --init --recursive
	mkdir -p lib/semver-utils/out
	cd lib/semver-utils && ./autogen.sh && ./configure --prefix $(abspath ./lib/semver-utils/out) && make && make install
	rm -f config.log lib/semver-utils/libsemver_config.h.in~

%.o: %.c
	$(CC) -c $^ -o $@ $(CFLAGS) $(INCLUDES)

bin/git-release: lib/libgit2/build/ lib/semver-utils/out/ $(OBJ) 
	mkdir -p bin
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(INCLUDES) $(LDFLAGS) $(LIBRARIES)
	chmod +x bin/git-release
	@echo
	@echo "Now run 'export LD_LIBRARY_PATH=$(abspath ./lib/libgit2/build/):$(abspath ./lib/semver-util/out)'"

.PHONY: all clean help clean-lib submodule
