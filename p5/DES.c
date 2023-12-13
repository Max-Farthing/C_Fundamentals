/** 
    @file DES.c
    @author Max Farthing (mafarthi)
    Implementation of the DES algorithm.
*/

#include "DES.h"
#include <stdio.h>
#include <string.h>
#include "DESMagic.h"
#include <stdlib.h>

/** Last index of C/D for circular shifting */
#define LASTIND 28
/** 2nd to last index of C/D for circular shifting */
#define LASTIND1 27
/** two index for SBox */
#define TWO 2
/** three index for SBOX */
#define THREE 3
/** five index for Sbox*/
#define FIVE 5
/** six index for Sbox*/
#define SIX 6
/** seven index for Sbox*/
#define SEVEN 7
/** sixteen index for Sbox*/
#define SIXTEEN 16

void prepareKey(byte key[BLOCK_BYTES], char const *textKey)
{
    size_t length = strlen(textKey);
    if(length > BLOCK_BYTES) {
        fprintf(stderr, "Key too long\n");
        exit(1);
    }
    memcpy(key, textKey, length);
    for(size_t i = length; i < BLOCK_BYTES; i++) {
        key[i] = 0x00;
    }
}

int getBit(byte const data[], int idx)
{
    byte res = (data[(idx - 1) / BYTE_SIZE] >> ((BYTE_SIZE - 1) - ((idx - 1) % BYTE_SIZE))) & 1;
    return res;
}

void putBit(byte data[], int idx, int val)
{
    if (val == 0) {
        data[(idx - 1) / BYTE_SIZE] &= ~(1 << ((BYTE_SIZE - 1) - ((idx - 1) % BYTE_SIZE)));
    } else {
        data[(idx - 1) / BYTE_SIZE] |= (1 << ((BYTE_SIZE - 1) - ((idx - 1) % BYTE_SIZE)));
    }
}

void permute(byte output[], byte const input[], int const perm[], int n)
{
    for (int i = 0; i < n; i++) {
        int permIndex = perm[i];
        int bitValue = getBit(input, permIndex);
        putBit(output, i + 1, bitValue);
    }

    if (n % BYTE_SIZE != 0) {
        byte padKey[BLOCK_BYTES];
        prepareKey(padKey, ""); // Use an empty string to pad with zeros
        int padBits = BYTE_SIZE - (n % BYTE_SIZE);
        for (int i = 0; i < padBits; i++) {
            int bitValue = getBit(padKey, i + 1);
            putBit(output, n + i + 1, bitValue);
        }
    }

 
}

/**
    Helper method for generate subkeys, helps shift all bits to the left
    by the said shift amount. Marked static for locality
    @param bits array of of bits to be shifted
    @param shift the number of shift operations needed
*/
static void shiftBitsLeft(byte bits[], int shift) {
    
    byte b1 = getBit(bits, 1);
    if(shift == 1) {
        for(int i = 1; i < SUBKEY_HALF_BITS; i++) {
            putBit(bits, i, getBit(bits, i + 1));
        }
        putBit(bits, LASTIND, b1);
    } else {
        byte b2 = getBit(bits, TWO);
        for(int i = 1; i < SUBKEY_HALF_BITS - 1; i++) {
            putBit(bits, i, getBit(bits, i + TWO));
        }
        putBit(bits, LASTIND1, b1);
        putBit(bits, LASTIND, b2);
    }
   
}

/**
    Helper method to combine two smaller subarrays into one large array.
    Marked static for locality, used by generateSubKeys, encryptBlock and
    DecryptBlock
    @param c first subarray to be added first
    @param d second subarray to be added on last
    @param combined the result of the two arrays being put together
    @param size future size of the larger sub array
*/
static void combineCD(byte c[], byte d[], byte combined[], int size) {
    for(int i = 0; i < size / TWO + 1; i++) {
        putBit(combined, i, getBit(c, i));
    }
    for(int i = size / TWO + 1; i < size + 1; i++) {
        putBit(combined, i, getBit(d, i - (size/TWO)));
    }
}

void generateSubkeys( byte K[ROUND_COUNT][SUBKEY_BYTES], byte const key[BLOCK_BYTES])
{
    byte c[SUBKEY_HALF_BITS], d[SUBKEY_HALF_BITS];
    permute(c, key, leftSubkeyPerm, SUBKEY_HALF_BITS);
    permute(d, key, rightSubkeyPerm, SUBKEY_HALF_BITS);

    for(int i = 1; i < ROUND_COUNT; i++) {
        shiftBitsLeft(c, subkeyShiftSchedule[i]);
        shiftBitsLeft(d, subkeyShiftSchedule[i]);

        byte combinedCD[SUBKEY_HALF_BITS * TWO];
        combineCD(c, d, combinedCD, SUBKEY_HALF_BITS * TWO);

        permute(K[i], combinedCD, subkeyPerm, SUBKEY_BITS);
    }
}



void sBox(byte output[1], byte const input[SUBKEY_BYTES], int idx)
{
    output[0] = 0;
    byte b1[1];
    for(int i = 1; i < SEVEN; i++) {
        putBit(b1, i, getBit(input, idx * SIX + i));
    }
    byte row[1];
    putBit(row, 1, getBit(b1, 1));
    putBit(row, TWO, getBit(b1, SIX));
    byte col[1];
    putBit(col, 1, getBit(b1, TWO));
    putBit(col, TWO, getBit(b1, THREE));
    putBit(col, THREE, getBit(b1, SBOX_OUTPUT_BITS));
    putBit(col, SBOX_OUTPUT_BITS, getBit(b1, FIVE));
    int c = (getBit(col, 1) << THREE) | (getBit(col, TWO) << TWO) | (getBit(col, THREE) << 1) | getBit(col, SBOX_OUTPUT_BITS);
    int r = (getBit(row, 1) << 1) | getBit(row, TWO);
    
    int h = sBoxTable[idx][r][c];
    for(int i = 0; i < SBOX_OUTPUT_BITS; i++) {
        putBit(output, i + 1, (h >> (SBOX_OUTPUT_BITS - 1 - i)) & 1);
    }
}

void fFunction(byte result[BLOCK_HALF_BYTES ], byte const R[BLOCK_HALF_BYTES ], byte const K[SUBKEY_BYTES])
{ 
    byte rExpanded[SBOX_INPUT_BITS];
    permute(rExpanded, R, expandedRSelector, SUBKEY_BITS);

    byte B[SBOX_INPUT_BITS];
    for(int i = 0; i < SBOX_INPUT_BITS; i++) {
        B[i] = rExpanded[i] ^ K[i];
    }
    
    byte sBoxOutput[SBOX_COUNT];
    for (int i = 0; i < BYTE_SIZE; i++) {
        sBox(&sBoxOutput[i], B, i);
    }
    byte sBoxFin[SBOX_OUTPUT_BITS];
    
    for(int i = 0; i < SBOX_COUNT; i++) {
        for(int j = 1; j < FIVE; j++) {
            putBit(sBoxFin, (i * SBOX_OUTPUT_BITS) + j, getBit(sBoxOutput, (i * BYTE_SIZE) + j));
        }
    }
    
    permute(result, sBoxFin, fFunctionPerm, BLOCK_HALF_BITS);
}

void encryptBlock(DESBlock *block, byte const K[ROUND_COUNT][SUBKEY_BYTES])
{
    byte leftPerm[BLOCK_HALF_BITS], rightPerm[BLOCK_HALF_BITS];
    permute(leftPerm, block->data, leftInitialPerm, BLOCK_HALF_BITS);
    permute(rightPerm, block->data, rightInitialPerm, BLOCK_HALF_BITS);

    for(int i = 1; i < ROUND_COUNT; i++) {
        byte temp[BLOCK_HALF_BITS];
        memcpy(temp, leftPerm, BLOCK_HALF_BITS);
        memcpy(leftPerm, rightPerm, BLOCK_HALF_BITS);
        fFunction(rightPerm, rightPerm, K[i]);
        for(int j = 0; j < BLOCK_HALF_BITS; j++) {
            rightPerm[j] = rightPerm[j] ^ temp[j];
        }
    }

    byte whole[BLOCK_BITS];
    combineCD(rightPerm, leftPerm, whole, BLOCK_BITS);
    

    permute(block->data, whole, finalPerm, BLOCK_BITS);
}

void decryptBlock(DESBlock *block, byte const K[ROUND_COUNT][SUBKEY_BYTES])
{ 
    byte leftPerm[BLOCK_HALF_BITS], rightPerm[BLOCK_HALF_BITS];
    permute(leftPerm, block->data, leftInitialPerm, BLOCK_HALF_BITS);
    permute(rightPerm, block->data, rightInitialPerm, BLOCK_HALF_BITS);

    int dec = SIXTEEN;
    for(int i = 1; i < ROUND_COUNT; i++) {
        byte temp[BLOCK_HALF_BITS];
        memcpy(temp, leftPerm, BLOCK_HALF_BITS);
        memcpy(leftPerm, rightPerm, BLOCK_HALF_BITS);
        fFunction(rightPerm, rightPerm, K[dec--]);
        for(int j = 0; j < BLOCK_HALF_BITS; j++) {
            rightPerm[j] = rightPerm[j] ^ temp[j];
        }
    }

    byte whole[BLOCK_BITS];
    combineCD(rightPerm, leftPerm, whole, BLOCK_BITS);
    

    permute(block->data, whole, finalPerm, BLOCK_BITS);
}