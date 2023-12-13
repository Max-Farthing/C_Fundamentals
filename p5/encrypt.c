/**
    @file encrypt.c
    @author Max Farthing (mafarthi)
    This file serves as the main component for encryption program. Contains main function used to
    read input file, perform encryption and write out the ciphertext output
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DESMagic.h"
#include "io.h"

/** number of expected args */
#define ARGS 4
/** arg number 2 */
#define ARG2 2
/** arg number 3 */
#define ARG3 3

/**
    Main method responsible for tying together encrypt program
    @param argc number of command line arguments
    @param argv array of char pointers from command line
    @return int based upon exit status
*/
int main(int argc,char *argv[]) 
{
    if(argc != ARGS) {
        fprintf(stderr, "usage: encrypt <key> <input_file> <output_file>\n");
        return 1;
    }

    char key[BLOCK_BYTES + 1];
    strcpy(key, argv[1]);
    byte binaryKey[BLOCK_BYTES];
    prepareKey(binaryKey, key);

    FILE *fp = fopen(argv[ARG2], "rb");
    if(!fp) {
        perror(argv[ARG2]);
        return 1;
    }

    FILE *out = fopen(argv[ARG3], "wb");
    if(!out) {
        fclose(fp);
        perror(argv[ARG3]);
        return 1;
    }

    byte subKeys[ROUND_COUNT][SUBKEY_BYTES];
    generateSubkeys(subKeys, binaryKey);

    DESBlock d;
    while(!feof(fp)) {
        readBlock(fp, &d);
        if(d.len % BYTE_SIZE != 0) {
            while(d.len < BYTE_SIZE) {
                d.data[d.len] = 0x00;
                d.len++;
            }
        }
        encryptBlock(&d, subKeys);

        writeBlock(out, &d);
        
    }
        

    fclose(fp);
    fclose(out);
    return 0;
}
