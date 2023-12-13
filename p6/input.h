/**
    @file input.h
    @author Max Farthing (mafarthi)
    This header files links the input source file to the rest of the project
*/

#include <stdio.h>
#include <stdlib.h>

/** doubles capacity of allowed dynamic storage */
#define RESIZE 2

/** initial capacity of line length */
#define INITIAL_CAP 10

/**
    This function reads one line of input at a time and returns a pointer to a dynamically allocated string
    @param fp file pointer
    @return char pointer to dynamically allocated string
*/
char *readLine(FILE *fp);