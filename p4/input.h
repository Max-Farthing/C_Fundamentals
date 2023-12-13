/**
    @file input.h
    @author Max Farthing (mafarthi)
    This header file allows the input file to interact and be used by other files in this project
*/

#include <stdio.h>
#include <stdlib.h>

/** doubles capacity of allowed dynamic storage */
#define RESIZE 2

/** initial capacity of line length */
#define INITIAL_CAP 10

/**
    This function is responsible for reading a single line of input from the given
    input stream and returning it as a string inside a block of dynamically allocated memory.
    @param fp a pointer to a file
    @return char pointer or a string of dynamically allocated memory
*/
char *readLine(FILE *fp);
