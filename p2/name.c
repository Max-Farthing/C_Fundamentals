/**
    @file name.c
    @author Max Farthing (mafarthi)
    This class handles the reading of the name field and converting it to standard format for output
*/

#include "name.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/** Exit status for a missing or badly formatted name. */
#define NAME_ERROR 101

/**
    This method read contents of name field from input and stores into a string, returns false
    if no name is found, exits 101 if name missing or too long
    @param name String of name read in from input
    @return boolean value whether name was read successfully
*/
bool read_name( char name[ FIELD_MAX + 1] )
{
    
    if(strlen(name) > FIELD_MAX || strlen(name) == 0){
        return false;
    }

    for(int i = 0; name[i]; i++){
        if(isdigit(name[i]) || name[i] == '-' || name[i] == '\''){
            return false;
        }
    }

    int spaceCount = 0;
    for(int i = 1; name[i]; i++) {
        if(name[i] == ' ' && isalpha(name[i - 1])){
            spaceCount++;
        }
    }
    if(spaceCount > 1){
        return false;
    }

    int spaceNum = 0;
    for(int i = 1; name[i]; i++) {
        if(isspace(name[i])){
            spaceNum++;
        }
    }

    if(spaceNum == 0) {
        return false;
    }

    return true;
}

/**
    This method converts string in name to standard format, exits with 101 if name is not 
    in acceptable format
    @param name String of name read in from input
*/
void fix_name( char name[ FIELD_MAX + 1 ] )
{
    bool hasComma = false;
    int i = skip_letters(name, 0);
    if(name[i] == ','){
        hasComma = true; //finding if name already has comma
    }
    // int spaces = 0;
    int firstSpace = 0;
    for(int i = 0; name[i]; i++){
      // if(isspace(name[i])){
      //   spaces++; //could possibly use for EC
      // }
        if(isspace(name[i]) && firstSpace == 0){
            firstSpace = i; //finding location of the first space
        } 
    }
    

    if(hasComma == true) {
        name[0] = toupper(name[0]); //if name already has comma just make sure first letters capitalized
        name[firstSpace + 1] = toupper(name[firstSpace + 1]); //first letter should be 1 after space
    } else {
        char first[ FIELD_MAX + 1 ];
        char last[ FIELD_MAX + 1];
        char full[ FIELD_MAX + 1];
        char replace[ FIELD_MAX + 1];

        copy_substring(first, 0, name, 0, firstSpace); //copy first name only
        copy_substring(last, 0, name, firstSpace + 1, strlen(name)); //copy last name only(extra space rn)
        first[0] = toupper(first[0]); //capitalize first chars
        last[0] = toupper(last[0]);
        copy_substring(full, 0, last, 0, strlen(last));//copy first name into full array
        int str_len = strlen(full);
        full[str_len] = ','; //add comma over null terminator
        replace[0] = ' '; //add missing space back through separate array
        copy_substring(replace, 1, first, 0, strlen(first)); //copy first name into new array with space
        copy_substring(full, strlen(last) + 1, replace, 0, strlen(replace)); //combine first and last name
        //copy full name into name array
        copy_substring(name, 0, full, 0, strlen(full));

        //if new format is over 30 chars throw error
        if(strlen(name) > FIELD_MAX){
            exit(NAME_ERROR);
        }
    }
}