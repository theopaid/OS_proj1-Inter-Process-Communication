# # This makefile builds and manages the project.

# PROGRAM_NAME = main

# CC = gcc

# Compilation flags
 CFLAGS  = -std=c99
 CFLAGS += -g
 CFLAGS += -Wall
 CFLAGS += -Wextra
 CFLAGS += -pedantic
 #CFLAGS += -Werror

 LDFLAGS = -lpthread
 LDFLAGS += -lcrypto

# # Test flags
# TEST_FLAGS  = -std=c99
# TEST_FLAGS += -Wall
# TEST_FLAGS += -I./test

# # Valgrind flags
VFLAGS  = --quiet
VFLAGS += --tool=memcheck
VFLAGS += --leak-check=full
VFLAGS += --error-exitcode=1
VFLAGS += --trace-children=yes

# SOURCE := $(shell find ./src -name '*.c' -not -name 'main.c')
# MAIN := 'src/main.c'

# SOURCES=$(shell find ./src -name '*.c')

# OBJECTS=$(SOURCES:.c=.o)

# EXECS=$(SOURCES:%.c=%)

######################

# .PHONY: usage
# usage:
# 	@echo "b|uild      Build the executable"
# 	@echo "c|lean      Remove any temporary products"
# 	@echo "cl|obber    Remove any generated files"
# 	@echo "m|emcheck   Check for memory leaks"
# 	@echo "r|un        Run 'build', then run the resulting executable"

# .PHONY: memcheck
# memcheck: build/test
# ifeq ( , $(shell which valgrind))
# 	$(error Please install valgrind to run this command)
# else
# 	@valgrind $(VFLAGS) ./build/test
# 	@echo "Memory check passed"
# endif
# .PHONY: m
# m: memcheck

# .PHONY: build
# build: $(SOURCE)
# 	mkdir -p build
# 	$(CC) $(CC_FLAGS) $(SOURCE) $(MAIN) -o ./build/$(PROGRAM_NAME)
# .PHONY: b
# b: build

# .PHONY: clean
# clean:
# 	rm -rf *.o *.out *.out.dSYM tmp*
# .PHONY: c
# c: clean

# .PHONY: clobber
# clobber: clean
# 	rm -rf build/
# .PHONY: cl
# cl: clobber

# .PHONY: run
# run: clean build
# 	./build/$(PROGRAM_NAME)
# .PHONY: r
# r: run

####################

# .PHONY: all
# all: $(OBJECTS) $(EXECS)

# .c:
# 	$(CC) $(CFLAGS) $< -o $@

# .o.: 
# 	$(CC) $^ $(LDFLAGS) -o $@

# .PHONY: clean
# clean:
# 	-@ $(RM) src/*.o 
# 	-@ $(RM) $(EXECS)

.PHONY: all
all: clean channel encoder1 encoder2 userInterface1 userInterface2

channel = src/channel.o src/sharedMemory.o src/utils.o
encoder1 = src/encoder1.o src/sharedMemory.o src/utils.o
encoder2 = src/encoder2.o src/sharedMemory.o src/utils.o
userInterface1 = src/userInterface1.o src/sharedMemory.o src/utils.o
userInterface2 = src/userInterface2.o src/sharedMemory.o src/utils.o

channel : $(channel)
		cc $(CC_FLAGS) -o channel $(channel) $(LDFLAGS)

encoder1 : $(encoder1)
		cc $(CC_FLAGS) -o encoder1 $(encoder1) $(LDFLAGS)

encoder2 : $(encoder2)
		cc $(CC_FLAGS) -o encoder2 $(encoder2) $(LDFLAGS)

userInterface1 : $(userInterface1)
		cc $(CC_FLAGS) -o userInterface1 $(userInterface1) $(LDFLAGS)

userInterface2 : $(userInterface2)
		cc $(CC_FLAGS) -o userInterface2 $(userInterface2) $(LDFLAGS)

run1:
	@valgrind $(VFLAGS) ./userInterface1 -p 27
	@echo "Memory check passed"

run2:
	@valgrind $(VFLAGS) ./userInterface2
	@echo "Memory check passed"

# src/channel.o : hdr/includes.h
# src/encoder1.o : hdr/includes.h
# src/encoder2.o : hdr/includes.h
# src/sharedMemory.o : hdr/includes.h
# src/userInterface1.o : hdr/includes.h
# src/userInterface2.o : hdr/includes.h
# src/utils.o : hdr/includes.h

.PHONY: clean
clean :
		@echo "Cleaning ..."
		rm -f channel $(channel)
		rm -f encoder1 $(encoder1)
		rm -f encoder2 $(encoder2)
		rm -f userInterface1 $(userInterface1)
		rm -f userInterface2 $(userInterface2)
