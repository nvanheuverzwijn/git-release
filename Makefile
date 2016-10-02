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

SRCDIR = src
SOURCES := $(shell find $(SRCDIR) -name *.c)
BUILDDIR = build
BINDIR = bin
OBJ = $(SOURCES:${SRCDIR}/%.c=${BUILDDIR}/%.o)


#
# PHONY target below
#

all: ${BINDIR} ${BUILDDIR} bin/git-release

clean: 
	rm -rf ${BUILDDIR}
	rm -rf ${BINDIR}

clean-lib:
	rm -rf lib/libgit2/build

help:
	@echo "help below"

#
# Files below
#

${BINDIR}:
	mkdir ${BINDIR}

${BUILDDIR}:
	mkdir ${BUILDDIR}

lib/libgit2/build/:
	git submodule update --init --recursive
	mkdir -p lib/libgit2/build
	cd lib/libgit2/build && cmake .. && cmake --build .

${BUILDDIR}/%.o: ${SRCDIR}/%.c
	$(CC) -c $^ -o $@ $(CFLAGS) $(INCLUDES)

bin/git-release: lib/libgit2/build/ $(OBJ) 
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(INCLUDES) $(LDFLAGS) $(LIBRARIES)
	chmod +x bin/git-release
	@echo
	@echo "Now run 'export LD_LIBRARY_PATH=$(abspath ./lib/libgit2/build/)'"

.PHONY: all clean help clean-lib submodule
