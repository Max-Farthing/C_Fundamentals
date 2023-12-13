/**
    @file ssn.c
    @author Max Farthing (mafarthi)
    This class serves the purpose of handling social security numbers and converting them to standard output
    for formatting
*/

#include "ssn.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/** Exit status for a missing or badly formatted ssn. */
#define SSN_ERROR 103

/** Number of digits in the first group in the SSN. */
#define START_DIGITS 3

/** Number of digits in the middle group in the SSN. */
#define MIDDLE_DIGITS 2

/** Number of digits in the last group in the SSN. */
#define END_DIGITS 4

/** Number of maximum allowed SSN chars */
#define MAX_SSNLENGTH 11

/**
    This function reads in SSN field from standard input and saves in a string, exits with
    103 status if SSN missing/too long
    @param ssn String storing the SSN from input
*/
void read_ssn( char ssn[ FIELD_MAX + 1] ) 
{

    bool naCheck = false;
    char na[] = "N/A";
    
    if(strcmp(ssn, na) == 0){
        naCheck = true; //check if string is "N/A";
        return;
    }

    if(strlen(ssn) > MAX_SSNLENGTH || strlen(ssn) < START_DIGITS){
        exit(SSN_ERROR); //check if longer than 11(with dashes) or shorter than 3
    }

    if(strlen(ssn) == 3 && naCheck == false){
        exit(SSN_ERROR); //if length is 3 and check is false means some other string than N/A
    }

    int dash;
    dash = skip_digits(ssn, 0); //find first non digit
    if(ssn[dash] == '\0' && strlen(ssn) > 9){ //checks if there are too many numbers
        exit(SSN_ERROR);
    }

    if(ssn[dash] != '-'  && ssn[dash] != '\0' && ssn[dash] != '/'){
        exit(SSN_ERROR); //checks if dash is an accepted character
    }
    
    if(ssn[dash] != '\0'){ //if dash is not the null terminator...(should be more dashes)
        int dash2 = skip_digits(ssn, dash + 1);
        if(ssn[dash2] != '-' && naCheck == false){ //check if not N/A then should be another dash
            exit(SSN_ERROR);
            int dash3 = skip_digits(ssn, dash2 + 1);
            if(ssn[dash3] != '\0'){ //last non digit should be null terminator
                exit(SSN_ERROR);
            }
        }
        
    }
}

/**
    This function converts SSN string to the standard format , exits with
    103 status if SSN missing/too long
    @param ssn String storing the SSN from input
*/
void fix_ssn( char ssn[ FIELD_MAX + 1] ) 
{
    if(strlen(ssn) == 9){
        char str[MAX_SSNLENGTH + 1];
        copy_substring(str, 0, ssn, 0, START_DIGITS); //copy first 3 nums
        str[3] = '-'; //add hyphen after first 3
        copy_substring(str, 4, ssn, START_DIGITS, 5); //copy next 2 nums
        str[6] = '-'; //add hyphen after first 3
        copy_substring(str, 7, ssn, 5, strlen(ssn)); //copy last 4 nums
        copy_substring(ssn, 0, str, 0, strlen(str));
    }
}