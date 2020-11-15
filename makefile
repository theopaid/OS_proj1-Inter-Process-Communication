# This makefile builds and manages the project.

PROGRAM_NAME = main

# Compilation flags
CFLAGS  = -std=c99
CFLAGS += -g
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -pedantic
#CFLAGS += -Werror

# Test flags
TEST_FLAGS  = -std=c99
TEST_FLAGS += -Wall
TEST_FLAGS += -I./test

# Valgrind flags
VFLAGS  = --quiet
VFLAGS += --tool=memcheck
VFLAGS += --leak-check=full
VFLAGS += --error-exitcode=1

SOURCE := $(shell find ./src -name '*.c' -not -name 'main.c')
MAIN := 'src/main.c'

.PHONY: usage
usage:
	@echo "b|uild      Build the executable"
	@echo "c|lean      Remove any temporary products"
	@echo "cl|obber    Remove any generated files"
	@echo "m|emcheck   Check for memory leaks"
	@echo "r|un        Run 'build', then run the resulting executable"

.PHONY: memcheck
memcheck: build/test
ifeq ( , $(shell which valgrind))
	$(error Please install valgrind to run this command)
else
	@valgrind $(VFLAGS) ./build/test
	@echo "Memory check passed"
endif
.PHONY: m
m: memcheck

.PHONY: build
build: $(SOURCE)
	mkdir -p build
	$(CC) $(CC_FLAGS) $(SOURCE) $(MAIN) -o ./build/$(PROGRAM_NAME)
.PHONY: b
b: build

.PHONY: clean
clean:
	rm -rf *.o *.out *.out.dSYM tmp*
.PHONY: c
c: clean

.PHONY: clobber
clobber: clean
	rm -rf build/
.PHONY: cl
cl: clobber

.PHONY: run
run: clean build
	./build/$(PROGRAM_NAME)
.PHONY: r
r: run

