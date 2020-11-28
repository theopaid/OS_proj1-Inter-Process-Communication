# # This makefile builds and manages the project.

# CC = gcc

# Compilation flags
 CFLAGS  = -std=c99
 CFLAGS += -g
 #CFLAGS += -Wall
 #CFLAGS += -Wextra
 #CFLAGS += -pedantic
 #CFLAGS += -Werror

 LDFLAGS = -lpthread
 LDFLAGS += -lcrypto

# # Valgrind flags
VFLAGS  = --quiet
VFLAGS += --tool=memcheck
VFLAGS += --leak-check=full
VFLAGS += --error-exitcode=1
VFLAGS += --trace-children=yes


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
