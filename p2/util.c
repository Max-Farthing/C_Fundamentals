/**
    @file util.c
    @author Max Farthing (mafarthi)
    This file serves the purpose of of manipulating strings to be used by other files
    in this program that are all a part of project 2
*/

#include "util.h"

#include <ctype.h>

/**
    This method is given a string and a starting index, looks through string and finds of the next
    char that is not a digit. Helps check formatting for SSN fields
    @param str given String in a char array
    @param start the provided starting index of string
    @return int an integer of first nondigit char
*/
int skip_digits( char str[], int start )
{
    int i = start;
    while(isdigit(str[i])){
        i++;
    }
    return i;
}

/**
    This method is given a string and starting index, looks through string and finds next char that is not
    a letter. Helps processing names
    @param str given string in char array
    @param start the provided starting index of string
    @return int an integer of first nondigit char
*/
int skip_letters( char str[], int start )
{
    int i = start;
    while(isalpha(str[i]) || str[i] == ' '){
      i++;
    }
    return i;
}

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
int copy_substring( char dest[], int dstart, char src[], int sstart, int send )
{
    int i = dstart;
    int j = sstart;

    while(j < send) {
      dest[i++] = src[j++];
    }

    dest[i] = '\0';
    return i;  
}