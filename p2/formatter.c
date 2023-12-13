/**
    @file formatter.c
    @author Max Farthing (mafarthi)
    This class serves the purpose of formatting all of the other given classes provided information and
    outputting to standard format as a finished product
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "name.h"
#include "date.h"
#include "ssn.h"

/** Birthday for people who will be 21 on the day this assignment is due. */
#define AGE_CUTOFF "2002-09-22"

/** Number of inputs */
#define INPUTS 3

/** Name Error */
#define NAME_ERROR 101

/** Name Error */
#define DATE_ERROR 102

/**
    Main method of program helping to tie up all the different classes and files and provide the expected output.
    @return int based upon the return status
*/
int main()
{
    
    char line[100];
    int totalIndividuals = 0;
    int maxNameLength = 0;
    int individuals21OrOver = 0;
    int individualsWithoutSSN = 0;

    while (scanf("%93[^\n]%*c", line) == 1) {
        totalIndividuals++;
        char name[FIELD_MAX + 1] = "";
        char date[FIELD_MAX + 1] = "";
        char ssn[FIELD_MAX + 1] = "";
        int num;
        // Parse the input line into name, date, and ssn fields using sscanf
        if ((num = sscanf(line, "%[^:]:%[^:]:%s", name, date, ssn) == INPUTS)) {
            // Process and format name, date, and ssn fields

            if(!read_name(name)){
                return NAME_ERROR;
            }
            read_date(date);
            read_ssn(ssn);


            fix_name(name);
            fix_date(date);
            fix_ssn(ssn);

            // Update statistics
            if (strlen(name) > maxNameLength) {
                maxNameLength = strlen(name);
            }

            if (strcmp(date, AGE_CUTOFF) <= 0) {
                individuals21OrOver++;
            }

            if (strcmp(ssn, "N/A") == 0) {
                individualsWithoutSSN++;
            }

            // Print the formatted output
            printf("%-30s %10s %11s\n", name, date, ssn);
        } else {
            return DATE_ERROR;
        }
    }

    // Print summary statistics
    printf("\nSummary\n");
    printf("-----------------------------------\n");
    printf("%-30s %4d\n", "Total individuals", totalIndividuals);
    printf("%-30s %4d\n", "Maximum name length", maxNameLength);
    printf("%-30s %4d\n", "Individuals 21 or over", individuals21OrOver);
    printf("%-30s %4d\n", "Individuals without an SS#", individualsWithoutSSN);
    return 0;


    
}