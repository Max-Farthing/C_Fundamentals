/**
    @file name.h
    @author Max Farthing (mafarthi)
    This files serves as a header for ssn.c as a way to connect it with other
    files in the project 2 program
*/

#include "util.h"
#include <stdbool.h>

/**
    This method read contents of name field from input and stores into a string, returns false
    if no name is found, exits 101 if name missing or too long
    @param name String of name read in from input
    @return boolean value whether name was read successfully
*/
bool read_name( char name[ FIELD_MAX + 1 ] );

/**
    This method converts string in name to standard format, exits with 101 if name is not 
    in acceptable format
    @param name String of name read in from input
*/
void fix_name( char name[ FIELD_MAX + 1 ] );
