# using the gcc compiler, and specifing our compiler flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror
# flags for valgrind
VALFLAGS = --leak-check=full -s
# flags for fsanitizer
FSANFLAGS = -fsanitize=address -g -pthread
# stating our src and object files
SRC = main.c house.c logger.c ghost.c hunter.c room.c threads.c utils.c evidence.c clean.c
OBJ = main.o house.o logger.o ghost.o hunter.o room.o threads.o utils.o evidence.o clean.o
# stating our header and exectuable file
DEPS = defs.h
EXECUTABLE = fp

# this compiles each source file into its object file
%.o: %.c $(DEPS) 
	$(CC) $(CFLAGS) -c -o $@ $<

# then this connects all of the object files together to our exectuable
$(EXECUTABLE): $(OBJ) 
	$(CC) $(CFLAGS) -o $@ $^

# an extra command to run valgrind on our executable 
valgrind:
	valgrind $(VALFLAGS) ./$(EXECUTABLE)

# if we want to remove all the .o files we created, we can just run this
clean: 
	rm -f $(OBJ) $(EXECUTABLE) $(EXECUTABLE)-san

# a build command to make the executable and remove object files
build: $(EXECUTABLE)
	rm -f $(OBJ)

fsan:
	$(CC) $(CFLAGS) $(FSANFLAGS) -o $(EXECUTABLE)-san $(SRC)
