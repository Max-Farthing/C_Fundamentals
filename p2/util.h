/**
    @file util.h
    @author Max Farthing (mafarthi)
    This file serves as a header file for the util.c file to help connect with other files in the program
*/

/** Maximum length of any field in the input. */
#define FIELD_MAX 30

/**
    This method is given a string and a starting index, looks through string and finds of the next
    char that is not a digit. Helps check formatting for SSN fields
    @param str given String in a char array
    @param start the provided starting index of string
    @return int an integer of first nondigit char
*/
int skip_digits( char str[], int start );

/**
    This method is given a string and starting index, looks through string and finds next char that is not
    a letter. Helps processing names
    @param str given string in char array
    @param start the provided starting index of string
    @return int an integer of first nondigit char
*/
int skip_letters( char str[], int start );

/**
    This method copies the original provided string from the selected start index up to but not including
    the end index. Null terminator is added after last character copied
    @param dest the String where src is being copied to
    @param dstart the starting index of the destination String(start of where copy is placed)
    @param src the string being copied
    @param sstart the start index of provided string to copy
    @param send index to be copied up to, not included
    @return int of the null index in dest[] 
*/
int copy_substring( char dest[], int dstart, char src[], int sstart, int send );
