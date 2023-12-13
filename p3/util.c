/**
    @file util.c
    @author Max Farthing (mafarthi)
    This class serves as a means to provide useful functions to other classes for various tasks such
    as checking for overflow
*/

#include "util.h"
#include <limits.h>

/**
    This function checks for overflow when adding two unsigned variables
    @param a unsigned long value
    @param b second unsigned long value
    @return boolean if overflow is detected
*/
bool checkAdd(unsigned long a, unsigned long b)
{
    unsigned long num;
    num = ULONG_MAX - a;
    if(b > num){
        return false;
    }
    return true;
}

/**
    This function checks for overflow when subtracting two unsigned variables(A - B)
    @param a unsigned long value
    @param b second unsigned long value
    @return boolean if overflow is detected
*/
bool checkSub(unsigned long a, unsigned long b)
{
    if(a - b > a){
        return false;
    }
    return true;
}

/**
    This function checks for overflow when multiplying two unsigned variables
    @param a unsigned long value
    @param b second unsigned long value
    @return boolean if overflow is detected
*/
bool checkMul(unsigned long a, unsigned long b)
{
    if(a == 0) {
        return true;
    }
    unsigned long x = ULONG_MAX / a;
    if(b > x){
        return false;
    }
    return true;
}

