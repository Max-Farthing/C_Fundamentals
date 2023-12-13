/**
    @file account.h
    @author Max Farthing (mafarthi)
    This header serves as a means to read and maintain the list of account names and balances and writing account file to output file
*/

#include <stdbool.h>
#include <stdio.h>

/** Maximum length of an account name. */
#define NAME_LIMIT 30

/** Limit on the length of an account file. */
#define AFILE_LIMIT 30

/** Limit on how many accounts are allowed in this program*/
#define ACCOUNT_LIMIT 100000

/** array to hold all balance values */
extern unsigned long balance[ACCOUNT_LIMIT];
/** array to hold all name values */
extern char names[][NAME_LIMIT + 1];

/**
    This function reads amount of currency from the given stream and stores it in the unsigned val
    @param fp pointer File location to read in input
    @param val unsigned val where currency is stored
    @return boolean if format is valid as unsigned long
*/
bool readCurrency(FILE *fp, unsigned long *val);

/**
    This function maintains a list of all account names and balances. Returns a pointer to the balance value
    for an account with the given name.
    @param name constant array with provided name
    @return pointer to account balance value from name. NULL if does not exist
*/
unsigned long* lookupAccount(char const name[ NAME_LIMIT + 1] );

/**
    This function loads all accounts from a given file with given account name. Checks for errors in filename
    or in contents
    @param fname File name
*/
void loadAccounts(char fname[AFILE_LIMIT + 1] );

/**
    This function writes out updated balance of all accounts to next version of the given account file
    @param fname File name
*/
void saveAccounts(char fname[AFILE_LIMIT + 1] );
