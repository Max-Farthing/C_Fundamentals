/** 
    @file DES.h
    @author Max Farthing (mafarthi)
    Header for the DES Implementation.
*/

#include "DESMagic.h"

/** Number of bits in a byte. */
#define BYTE_SIZE 8

/** Round a number of bits up to the nearest number of bytes needed 
    to store that many bits. */
#define ROUND_TO_BYTES( bits ) (((bits) + BYTE_SIZE - 1)/BYTE_SIZE)

/** Number of bytes in a DES block. */
#define BLOCK_BYTES ROUND_TO_BYTES( BLOCK_BITS )

/** Number of bytes in the left or right halves of a block (L and R). */
#define BLOCK_HALF_BYTES ROUND_TO_BYTES( BLOCK_HALF_BITS )

/** Number of bytes to store the left-side and right-side values (C
    and D) used to create the subkeys. */
#define SUBKEY_HALF_BYTES ROUND_TO_BYTES( SUBKEY_HALF_BITS )

/** Number of bytes to store a whole subkey (K_1 .. K_16). */
#define SUBKEY_BYTES ROUND_TO_BYTES( SUBKEY_BITS )

/** Type used to represent a block to encrypt or decrypt with DES. */
typedef struct {
  /** Sequence of bytes in the block. */
  byte data[ BLOCK_BYTES ];

  /** Number of bytes currently in the data array (e.g., the last block in a file could
      be shorter. */
  int len;
} DESBlock;

/**
    This function checks the given text key to make sure its not too long. Copies the 
    characters of this key from textKey to the key array and pads with zero bytes 
    up to the length of a DES block.
    @param key array of bytes
    @param textKey key in text format
*/
void prepareKey( byte key[ BLOCK_BYTES ], char const *textKey );

/**
    This function returns zero or one based on the value of the bit at index idx in the given array of bytes.
    @param data an array of bytes
    @param idx the index to check for value
    @return int 1 or 0 based on the value at idx
*/
int getBit( byte const data[], int idx );

/**
    This function clears (if val is zero) or sets (if val is one) the bit at index idx of the data array
    @param data an array of bytes
    @param idx the index to check for value
*/
void putBit( byte data[], int idx, int val );

/**
    This function performs the permute operation, copying n bits from the given input array to output selected 
    by the first n elements of perm. If n isn’t multiple of 8, then this function should set any remaining bits 
    in the last byte to zero
    @param output output of bytes
    @param input input of bytes
    @param perm array used for copying
    @param n the amount of bits to be copied
*/
void permute( byte output[], byte const input[], int const perm[], int n );

/**
    This function computes 16 subkeys based on the input key and stores each one in an element of the given K array.
    The resulting subkeys are stored in K[ 1 ] .. K[ 16 ]. Element zero of the subkey array isn’t used.
    @param K 2d array of bytes
    @param key array of bytes
*/
void generateSubkeys( byte K[ ROUND_COUNT ][ SUBKEY_BYTES ], byte const key[ BLOCK_BYTES ] );

/**
    This function returns the result of an S-Box calculation in the four high-order bits of output[ 0 ].
    The idx value ranges from 0 to 7. The result is an element of sBoxTable[ idx ] selected by bits 
    idx * 6 + 1 to idx * 6 + 6 of B
    @param output the output byte array
    @param input the input byte array
    @param idx value of index
*/
void sBox( byte output[ 1 ], byte const input[ SUBKEY_BYTES ], int idx );

/**
    This computes the f function based on the given 32-bit value R and the given 48-bit subkey, S.
    The result is stored in the result array.
    @param result result of f function
    @param R 32 bit value
    @param K 48 bit subkey value
*/
void fFunction( byte result[ BLOCK_HALF_BYTES ], byte const R[ BLOCK_HALF_BYTES ], byte const K[ SUBKEY_BYTES ] );

/**
    This function performs the encrypt operation on the byte array in block, using the subkeys in the K array.
    The encrypted result is stored back in the given block.
    @param block pointer to DESBlock
    @param k 2d array of bytes
*/
void encryptBlock( DESBlock *block, byte const K[ ROUND_COUNT ][ SUBKEY_BYTES ] );

/**
    This function performs the decrypt operation on the byte array in block, using the subkeys in the K array.
    The encrypted result is stored back in the given block.
    @param block pointer to DESBlock
    @param k 2d array of bytes
*/
void decryptBlock( DESBlock *block, byte const K[ ROUND_COUNT ][ SUBKEY_BYTES ] );

