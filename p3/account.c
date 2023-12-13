/**
    @file account.c
    @author Max Farthing (mafarthi)
    This class serves as a means to read and maintain the list of account names and balances and writing account file to output file
*/

#include "account.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"
#include <limits.h>

/** Number of accounts supported by the program. */
#define ACCOUNT_LIMIT 100000

/** multiplying/dividing by 10 moves decimal place once*/
#define ONE_DEC_PLACE 10

/** multiplying/dividing by 100 moves decimal place twice */
#define TWO_DEC_PLACE 100

/** maximum allowed decimal digits */
#define MAX_DEC_DIGITS 2

/** last number to be a single digit integer */
#define SINGLE_DIGIT 9

/** array to hold all balances as unsigned longs */
unsigned long balance[ACCOUNT_LIMIT];
/** array to hold names of all account holders */
char names[ACCOUNT_LIMIT][NAME_LIMIT + 1];
/** the number of accounts being read in(so we know how many times to iterate)*/
int numAccounts = 0;

/**
    This function serves as a helper function to find the last digit in a file name. Its purpose is to help
    read in file names.
    @param fname file name from command line argument
    @return integer of position of the last digit of the file
*/
int findLastDigit(char fname[AFILE_LIMIT + 1])
{
    for(int i = 0; fname[i]; i++){
        if(isdigit(fname[i]) && fname[i + 1] == '.'){ //if digit and the next digit is a '.' that means this is last digit in filename
            return i;
        }
    }
    return 0;
}

/**
    This function reads amount of currency from the given stream and stores it in the unsigned val
    @param fp pointer File location to read in input
    @param val unsigned val where currency is stored
    @return boolean if format is valid as unsigned long
*/
bool readCurrency( FILE *fp, unsigned long *val)
{
    unsigned long value = 0;
    bool dec = false;
    char ch = fgetc(fp);
    int decDigits = 0;
    bool flag = false; 

    while(ch != EOF){ 
        if (ch == '.') {
            dec = true; //if comma set bool to true to count decimal digits
            ch = fgetc(fp);
        } else if (isdigit(ch)){
            flag = true; //set true after first digit so we know to stop reading when space occurs after number
            unsigned long d = ch - '0';

            if(checkMul(value, ONE_DEC_PLACE)){
                value *= ONE_DEC_PLACE;
            } else {
                return false;
            }
            if(checkAdd(value, d)) {
                value += d;
            } else {
                return false;
            }

            if(dec == true) {
                decDigits++; //counting decimal digits
            }
            ch = fgetc(fp);
        } else if(isspace(ch) || ch == '\n') {
            if(flag == true){
                break; //if space or newline character is reached after a number break loop
            }
            ch = fgetc(fp);
        } else {
            if(isalpha(ch)){
                return false; //checking for any letters in number
            } 
        }
        
    }
    if(decDigits > MAX_DEC_DIGITS) {
        return false; //check if there are more than 2 decimals
    }

    *val = value; //assign value return true
    return true;
}

/**
    This function maintains a list of all account names and balances. Returns a pointer to the balance value
    for an account with the given name.
    @param name constant array with provided name
    @return pointer to account balance value from name. NULL if does not exist
*/
unsigned long* lookupAccount(char const name[ NAME_LIMIT + 1] )
{
    unsigned long* ret = NULL;
    int i = 0;
    while(i < numAccounts) {
        if(strcmp(names[i], name) == 0){ //if name matches name in array of account names
            ret = &balance[i]; //return pointer to value in balance array
            return ret;
        }
        i++;
    }
    return ret;
}

/**
    This function loads all accounts from a given file with given account name. Checks for errors in filename
    or in contents
    @param fname File name
*/
void loadAccounts(char fname[AFILE_LIMIT + 1] )
{
    
    if(strlen(fname) > AFILE_LIMIT) { //if file name is too long
            fprintf(stderr, "Invalid account file name: %s\n", fname);
            exit(1);
    }
    bool dash = false;
    for(int i = 0; fname[i]; i++) { //various types of error checking in file name
         if(fname[i] == '-') {
            dash = true;
        }
        if(dash == false && !isalpha(fname[i]) && !isdigit(fname[i]) && fname[i] != '_') {
            fprintf(stderr, "Invalid account file name: %s\n", fname);
            exit(1);
        }
        if(fname[i] == '.' && fname[i - 1] == '9' && strlen(fname) >= AFILE_LIMIT) {
            fprintf(stderr, "Invalid account file name: %s\n", fname);
            exit(1);
        }
    }
    if(findLastDigit(fname) == 0){ //if file name does not have digit
        fprintf(stderr, "Invalid account file name: %s\n", fname);
        exit(1);
    }

    FILE *file = fopen(fname, "r"); //open file for reading
    if(!file) {
        fprintf(stderr, "Invalid account file name: %s\n", fname);
        exit(1);
    }

    char str[NAME_LIMIT + 1];
    unsigned long bal;
    int count = 0;
    int scan = 0;
    
    while((scan = fscanf(file, "%31s", str)) != EOF) { //read in name of customer from file

        if(readCurrency(file, &bal)){ //read in currency from file using readcurrency function
            scan++;
        }
        
        if(scan != MAX_DEC_DIGITS) {
            fprintf(stderr, "Invalid account file\n"); //something was not read correctly
            exit(1);
        }
        if(strlen(str) > NAME_LIMIT){ //if name is too long
            fprintf(stderr, "Invalid account file\n");
            exit(1);
        }

        if(bal < 0 || bal > ULONG_MAX) { //boundary value checkss
            fprintf(stderr, "Invalid account file\n");
            exit(1);
        }
        strcpy(names[count], str); //copy name into names array
        balance[count] = bal; //copy balance into balance array
        count++; //increment array position by 1
    }
    numAccounts = count; //sets the total number of accounts
}

/**
    This function writes out updated balance of all accounts to next version of the given account file
    @param fname File name
*/
void saveAccounts(char fname[AFILE_LIMIT + 1] )
{
    int lastDigit = findLastDigit(fname); //find last digit of file
    char str1[AFILE_LIMIT];
    char str2[AFILE_LIMIT];
    strncpy(str1, fname, lastDigit + 1);

    int count = 0;
    for(int i = lastDigit + 1; fname[i]; i++){
        str2[count] = fname[i];
        count++;
    }
    str2[count] = '\0';

    if(fname[lastDigit] == '9'){ //if else statements correctly write the new output files name
        str1[lastDigit] = '1';
        str1[lastDigit + 1] = '0';
        str1[lastDigit + MAX_DEC_DIGITS] = '\0';
        strcat(str1, str2);
    } else {
        int num = fname[lastDigit] - '0';
        num++;
        str1[lastDigit] = num + '0';
        str1[lastDigit + 1] = '\0';
        strcat(str1, str2);
    }

    FILE *fp = fopen(str1, "w"); //open new file to write
    if(!fp){
        printf("error saving\n");
        exit(1);
    }

    for(int i = 0; i < numAccounts; i++) { //write out new account files using array information
        if(names[i][0] != '\0'){
            unsigned long decimals = balance[i] % TWO_DEC_PLACE;
            unsigned long integer = balance[i] / TWO_DEC_PLACE;
            if(decimals >= 0 && decimals <= SINGLE_DIGIT){
                fprintf(fp, "%-30s%19lu.0%lu\n", names[i], integer, decimals);
            } else {
                fprintf(fp, "%-30s%19lu.%lu\n", names[i], integer, decimals);
            }
        }
    }

    fclose(fp);
}