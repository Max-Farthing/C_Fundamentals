/**
    @file decrypt.c
    @author Max Farthing (mafarthi)
    This file serves as the main component for the decrypt program. Contains main function and other
    uses other components to read input file and perform decryption and write out plaintext output
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
    Main method responsible for tying together decrypt program
    @param argc number of command line arguments
    @param argv array of char pointers from command line
    @return int based upon exit status
*/
int main(int argc, char *argv[]) 
{
    if(argc != ARGS) {
        fprintf(stderr, "usage: decrypt <key> <input_file> <output_file>\n");
        return 1;
    }

    if(strlen(argv[1]) > BYTE_SIZE) {
        fprintf(stderr, "Key too long\n");
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
    while(1) {
        readBlock(fp, &d);

        if(d.len <= 0) {
            break;
        }
        if (d.len % BYTE_SIZE != 0) {
            fprintf(stderr, "Bad ciphertext file length: %s\n", argv[ARG2]);
            fclose(fp);
            fclose(out);
            return 1;
        }
        decryptBlock(&d, subKeys);
        while(d.len > 0 && d.data[d.len - 1] == 0) {
            d.len--;
        }
        writeBlock(out, &d);
        d.len = 0;
    }

    fclose(fp);
    fclose(out);
    return 0;
}