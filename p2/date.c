/**
    @file date.c
    @author Max Farthing (mafarthi)
    This class serves the purpose of reading and converting dates to standard format for output
    in project 2
*/

#include "date.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/** Exit status for a missing or badly formatted date. */
#define DATE_ERROR 102

/** Two low-order digits of the current year. */
#define CURRENT_YEAR "23"

/** Number of digits in the short (2 digit) year. */
#define SHORT_YEAR 2

/** Number of digits in the full (4 digit) year. */
#define FULL_YEAR 4

/** Number of digits in the month. */
#define MONTH_DIGITS 2

/** Number of digits in the day. */
#define DAY_DIGITS 2

/** Number of digits in the day. */
#define DATE_LENGTH 11

/** Number of digits in the day. */
#define DATE_TIL_MONTH 8

/** Number of digits in the day. */
#define DATE_TIL_DAY 6

/** Number of digits in the day. */
#define DATE_TIL_DAY 6

/**
    Helper method to help find the separations of the first date digit chars in the date array,
    Very similar to skipDigits.
    @param str string array to count
    @param start starting index
    @return int of number of counted spaces that have passed until non digit
*/
int date_helper(char str[], int start)
{
    int i = start;
    int count = 0;
    while(isdigit(str[i])){
        i++;
        count++;
    }
    return count;
}

/**
    This function reads contents of date field and stores in a string, exits with 102 status
    if date missing/too long
    @param date String of date read in from input
*/
void read_date( char date[ FIELD_MAX + 1] ) 
{
    int i = 0;
    char ch;

    // Skip whitespace and any separator characters at the beginning
    while ((ch = date[i]) != '\0' && (isspace(ch) || ch == '/' || ch == '-')) {
        i++;
    }

    // Read characters until we reach the end of the date field or field max length
    while ((ch = date[i]) != '\0' && !isspace(ch) && ch != ':' && i < FIELD_MAX) {
        date[i++] = ch;
    }

    // Null-terminate the date string
    date[i] = '\0';

    // Check if the date is empty
    if (i == 0) {
        // Handle the case where the date is missing
        exit(DATE_ERROR);
    }

    // Count the number of separator characters (hyphens or slashes)
    int separatorCount = 0;
    for (int j = 0; j < i; j++) {
        if (date[j] == '/' || date[j] == '-') {
            separatorCount++;
        }
    }

    // Check if the date contains exactly two separator characters
    if (separatorCount != SHORT_YEAR) {
        exit(DATE_ERROR);
    }

    // Validate that the characters in date are either digits or separator characters
    for (int j = 0; j < i; j++) {
        if (date[j] != '/' && date[j] != '-' && !isdigit(date[j])) {
            exit(DATE_ERROR);
        }
    }

    
}

/**
    This function converts date string to standard format, exits with 102 status if string
    is not acceptable
    @param date String of date read in from input
*/
void fix_date( char date[ FIELD_MAX + 1 ] )
{
    char year[FULL_YEAR + 1];
    char month[FULL_YEAR];
    char day[FULL_YEAR];
    char overall[DATE_LENGTH];
    int i = date_helper(date, 0);
    int j = date_helper(date, i + 1);
    int k = date_helper(date, i + j + SHORT_YEAR);
    if(i > k) { //if year is first, will K always be greater?
        copy_substring(year, 0, date, 0, FULL_YEAR);
        copy_substring(month, 0, date, FULL_YEAR + 1, DATE_TIL_DAY + 1);
        copy_substring(day, 0, date, DATE_TIL_MONTH, DATE_LENGTH - 1);

        copy_substring(overall, 0, year, 0, FULL_YEAR);
        overall[4] = '-';
        copy_substring(overall, FULL_YEAR + 1, month, 0, SHORT_YEAR);
        overall[7] = '-';
        copy_substring(overall, DATE_TIL_MONTH, day, 0, SHORT_YEAR);
        copy_substring(date, 0, overall, 0, DATE_LENGTH - 1);
    } else if(k > i) { //if year is third
        copy_substring(month, 0, date, 0, SHORT_YEAR);
        copy_substring(day, 0, date, FULL_YEAR - 1, FULL_YEAR + 1);
        copy_substring(year, 0, date, DATE_TIL_DAY, DATE_LENGTH - 1);

        copy_substring(overall, 0, year, 0, FULL_YEAR);
        overall[4] = '-';
        copy_substring(overall, FULL_YEAR + 1, month, 0, SHORT_YEAR);
        overall[7] = '-';
        copy_substring(overall, DATE_TIL_MONTH, day, 0, SHORT_YEAR);
        copy_substring(date, 0, overall, 0, DATE_LENGTH - 1);
    } else { //if year and month/day are equal
        copy_substring(month, 0, date, 0, SHORT_YEAR);
        copy_substring(day, 0, date, FULL_YEAR - 1, FULL_YEAR + 1);
        copy_substring(year, 0, date, DATE_TIL_DAY, DATE_TIL_MONTH);
        int yearVal = atoi(year); //get value for year and check if 1900 or 2000's
        char newYear[FULL_YEAR + 1];
        if(yearVal >= 24){
            newYear[0] = '1';
            newYear[1] = '9';
            newYear[SHORT_YEAR] = '\0';

        } else {
            newYear[0] = '2';
            newYear[1] = '0';
            newYear[SHORT_YEAR] = '\0';

        }
        year[SHORT_YEAR] = year[0];
        year[SHORT_YEAR + 1] = year[1];
        year[0] = newYear[0];
        year[1] = newYear[1];
        year[FULL_YEAR] = '\0';
        copy_substring(overall, 0, year, 0, FULL_YEAR);
        overall[4] = '-';
        copy_substring(overall, FULL_YEAR + 1, month, 0, SHORT_YEAR);
        overall[7] = '-';
        copy_substring(overall, DATE_TIL_MONTH, day, 0, SHORT_YEAR);
        copy_substring(date, 0, overall, 0, DATE_LENGTH - 1); //need to convert from 2 to 4 years
    }
}