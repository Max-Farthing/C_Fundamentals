/**
    @file io.h 
    @author Max Farthing
    This header file is responsible for connecting io.c to other components in the program
*/

#include <stdio.h>
#include "DES.h"
#include "DESMagic.h"

/**
    This function reads up to 8 bytes from the given input file, storing them in the data array
    of block and setting the len field to indicate how many bytes have been read.
    @param fp file pointer
    @param block pointer to DESBlock
*/
void readBlock( FILE *fp, DESBlock *block );

/**
    This function writes the contents of the data array in block to the given file.
    The len field of block indicates how many bytes the block contains.
    @param fp file pointer
    @param block pointer to DESBlock
*/
void writeBlock( FILE *fp, DESBlock const *block );