/**
    @file io.c 
    @author Max Farthing
    This file is responsible for reading input and writing output files a block at a time
*/

#include "io.h"
#include <stdio.h>
#include "DESMagic.h"

void readBlock( FILE *fp, DESBlock *block )
{
    block->len = fread(block->data, sizeof(byte), BLOCK_BYTES, fp);
}

void writeBlock( FILE *fp, DESBlock const *block )
{
    fwrite(block->data, sizeof(byte), block->len, fp);
}