/**
    @file util.h
    @author Max Farthing (mafarthi)
    This header serves as a means to provide useful functions to other classes for various tasks such
    as checking for overflow
*/

#include <stdio.h>
#include <stdbool.h>

/**
    This function checks for overflow when adding two unsigned variables
    @param a unsigned long value
    @param b second unsigned long value
    @return boolean if overflow is detected
*/
bool checkAdd(unsigned long a, unsigned long b);

/**
    This function checks for overflow when subtracting two unsigned variables
    @param a unsigned long value
    @param b second unsigned long value
    @return boolean if overflow is detected
*/
bool checkSub(unsigned long a, unsigned long b);

/**
    This function checks for overflow when multiplying two unsigned variables
    @param a unsigned long value
    @param b second unsigned long value
    @return boolean if overflow is detected
*/
bool checkMul(unsigned long a, unsigned long b);
