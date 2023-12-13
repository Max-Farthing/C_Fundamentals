/**
    @file transaction.c
    @name Max Farthing (mafarthi)
    This class is responsible for reading and processing all the transactions in the transaction file
*/

#include "transaction.h"
#include "account.h"
#include <string.h>
#include <stdlib.h>
#include "util.h"
#include <limits.h>

/** characters needed to buy or sell */
#define BUY_SELL 5
/** scanner reading in 3 things */
#define SCAN_LIM 3
/** transaction file reading in 4 things */
#define TRANS_LIM 4

/**
    This function reads and performs all transactions in the transaction file with the given name
    @param fname array storing file name
*/
void processTransactions(char const fname[])
{
    char name[NAME_LIMIT + 1]; //all variables initialized to help with transaction processing
    unsigned long amount = 0;
    char action[BUY_SELL];
    unsigned long value = 0;
    unsigned long total = 0;
    int scan = 0;
    FILE *fp = fopen(fname, "r"); //open file to read from
    if(!fp) {
        printf("transaction error");
        exit(1);
    }
   
    while((scan = fscanf(fp, "%s%s%lu", name, action, &amount)) == SCAN_LIM) {
        if(readCurrency(fp, &value)) { //read in value as currency format
            scan++;
        }
        if(scan != TRANS_LIM) {
            fprintf(stderr, "invalid transaction file\n"); //if any values not read in correctly
            exit(1);
        }
        if(!checkMul(amount, value)){
            fprintf(stderr, "Account overflow\n"); //checking for overflow with total value(1 of test cases)
            exit(1);
        }
        total = (amount * value); //calculate total from amount of shares * value of shares
        unsigned long *p = lookupAccount(name); //finding balance value

        if(!p){
            fprintf(stderr, "Invalid transaction file\n"); //check if pointer not assigned
            exit(1);
        }

        if(*p < 0 || *p > ULONG_MAX) {
            fprintf(stderr, "Account overflow\n"); //check valid boundaries
            exit(1);
        }

        if(strcmp(action, "buy") == 0) {
            if(!checkSub(*p, total)) {
                fprintf(stderr, "Account overflow\n"); //check if overflow from buying stock
                exit(1);
            }
            *p -= total; //subtract total from balance value
            
            if(*p < 0) {
                fprintf(stderr, "Account overflow\n"); //check boundary value
                exit(1);
            }
        } else if (strcmp(action, "sell") == 0){
            if(!checkAdd(*p, total)) {
                fprintf(stderr, "Account overflow\n"); //check for overflow when selling stock
                exit(1);
            }
            *p += total; //add total to balance value
            
            if(*p > ULONG_MAX) {
                fprintf(stderr, "Account overflow\n"); //check boundary value
                exit(1);  
            }

        } else {
            fprintf(stderr, "Invalid transaction file"); //if not buy or sell then exit
            exit(1);
        }

    }

    fclose(fp);
}

