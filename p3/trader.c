/**
    @file trader.c
    @author Max Farthing (mafarthi)
    This class contains the main method which is responsible for tying the program together as well reading
    from command line arguments to help the program run smoothly.
*/

#include <stdlib.h>
#include <stdio.h>
#include "account.h"
#include "transaction.h"

/** Index of the account file command-line argument. */
#define ACCOUNT_ARG 1

/** Index of the transacton file command-line argument. */
#define TRANSACT_ARG 2

/** number of arguments expected */
#define ARG_TOTAL 3

/**
    main function responsible for tying the program together and returns an exit status based upon completion
    @return int based upon exit status
*/
int main(int argc, char *argv[])
{
    if(argc != ARG_TOTAL){ //program requires 3 command line arguments, if not 3 then return error
        fprintf(stderr, "usage: trader <account-file> <transaction-file>\n"); 
        return 1;
    }

    char *account = argv[ACCOUNT_ARG];
    FILE *accFile = fopen(account, "r"); //open file pointer to account command line argument
    if(!accFile) {
        fprintf(stderr, "Can't open account file: %s\n", account); //if pointer is null return error
        return 1;
    }

    loadAccounts(account); //load account values into global array and check file name formatting
    
    char *transaction = argv[TRANSACT_ARG]; 
    FILE *transFile = fopen(transaction, "r"); //open file for transactions
     if(!transFile) {
        fprintf(stderr, "Can't open transaction file: %s\n", transaction); //if pointer is null return error
        return 1;
    }


    processTransactions(transaction); //process transactions: adding/subtracting from balance values
    saveAccounts(account); //save accounts and write output to new file


    fclose(accFile);
    fclose(transFile); //close all opened files
    return 0;
}