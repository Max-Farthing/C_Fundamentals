/**
    @file name.c
    @author Max Farthing (mafarthi)
    This file serves as a header file for date.c as a way to connect with the other files in the 
    project 2 program
*/

#include "util.h"
#include <stdbool.h>

/**
    This function reads contents of date field and stores in a string, exits with 102 status
    if date missing/too long
    @param date String of date read in from input
*/
void read_date( char date[ FIELD_MAX + 1] );

/**
    This function converts date string to standard format, exits with 102 status if string
    is not acceptable
    @param date String of date read in from input
*/
void fix_date( char date[ FIELD_MAX + 1 ] );

