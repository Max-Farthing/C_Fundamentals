/**
    @file value.c
    @author Max Farthing (mafarthi)
    Implementation for the value component, with support for integer
    and (eventually) string values.
  */

#include "value.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
/** used inside of parse string to null terminate */
#define NULL_TERM 2

//////////////////////////////////////////////////////////
// Integer implementation.

// print method for Integer.
static void printInteger( Value const *v )
{
  // Print the integer inside this value.
  printf( "%d", v->ival );
}

// move method for Integer.
static void moveInteger( Value const *src, Value *dest )
{
  // We just need to copy the value.
  dest->ival = src->ival;
  
  dest->print = src->print;
  dest->move = src->move;
  dest->equals = src->equals;
  dest->hash = src->hash;
  dest->empty = src->empty;
}

// equals method for Integer.
static bool equalsInteger( Value const *v, Value const *other )
{
  // Make sure the other object is also an Integer.
  // (i.e., it uses the same print funtion)
//   if ( other->print != printInteger )
//     return false;

  return v->ival == other->ival;
}

// hash method for Integer.
static unsigned int hashInteger( Value const *v )
{
  // This will overflow negative values to large positive ones.
  return v->ival;
}

// Free memory used inside this integer Value.
static void emptyInteger( Value *v )
{
  // An int vaue doesn't need any additional memory.
}

int parseInteger( Value *v, char const *str )
{
  // Try to parse an integer from str.
  int val, len;
  if ( sscanf( str, "%d%n", &val, &len ) != 1 )
    return 0;

  // Fill in all the fields of v for an integer type of value.
  v->print = printInteger;
  v->move = moveInteger;
  v->equals = equalsInteger;
  v->hash = hashInteger;
  v->empty = emptyInteger;
  v->ival = val;

  // Return how much of str we parsed.
  return len;
}

//////////////////////////////////////////////////////////
// String implementation.

/**
    Print method for printing string values
    @param v const value struct pointer
*/
static void printString( Value const *v )
{
    printf("%s", (char *)v->vptr);
}

static void moveString( Value const *src, Value *dest )
{
    // We just need to copy the value.
    dest->vptr = src->vptr;
    
    dest->print = src->print;
    dest->move = src->move;
    dest->equals = src->equals;
    dest->hash = src->hash;
    dest->empty = src->empty;

}

static bool equalsString( Value const *v, Value const *other )
{
    // if ( other->print != printString )
    //     return false;
    
    return strcmp((char *)v->vptr, (char *)other->vptr) == 0;
}

static unsigned int hashString( Value const *v )
{    
    size_t i = 1;
    unsigned int hash = 0;
    char *str = (char *)v->vptr;
    size_t length = strlen(str) - 1;
    while (i != length) {
        hash += str[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

static void emptyString( Value *v )
{
    free(v->vptr);
}

int parseString( Value *v, char const *str )
{
    int len = 0; //iterate through string

    while (isspace(str[len])) { //iterate through leading whitespace
        len++;
    }

    if(str[len] != '\"') {
        return 0;
    }
    int count = 0; //counts number of letters consumed after reading in spaces
    int holder = len; //placeholder for length after reading in original spaces
    while (str[len] != '\0') {
        count++;
        len++;
        if(str[len] == '\"') {
            break;
        }
    }

    char *st2 = (char *)malloc(count + NULL_TERM);

    strncpy(st2, str + holder, count + 1); //copy after leading white space with ptr arithmetic
    st2[count + 1] = '\0';

    v->print = printString;
    v->move = moveString;
    v->equals = equalsString;
    v->hash = hashString;
    v->empty = emptyString;
    v->vptr = st2;

    return holder + count + 1; //return number of leading spaces + til last \ and add 1
}
