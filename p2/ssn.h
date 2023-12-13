/**
    @file ssn.h
    @author Max Farthing (mafarthi)
    This file serves as a header file for ssn.c as a way to connect to other files throughout
    project 2
*/

#include "util.h"
#include <stdbool.h>

/** String indicating no SSN */
#define SSN_NA "N/A"

/**
    This function reads in SSN field from standard input and saves in a string, exits with
    103 status if SSN missing/too long
    @param ssn String storing the SSN from input
*/
void read_ssn( char ssn[ FIELD_MAX + 1] );

/**
    This function converts SSN string to the standard format , exits with
    103 status if SSN missing/too long
    @param ssn String storing the SSN from input
*/
void fix_ssn( char ssn[ FIELD_MAX + 1] );
