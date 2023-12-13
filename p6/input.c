/**
    @file input.c
    @author Max Farthing (mafarthi)
    This file handles the input of the user specifically reading the line input provided
*/

#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

char *readLine(FILE *fp)
{
    char *line = NULL;
    char ch;
    int size = 0;
    int cap = INITIAL_CAP; //starting capacity
    line = (char *)malloc(cap); //allocate line memory with starting capacity
    while((ch = fgetc(fp)) != '\n' && ch != EOF) {
        if(size >= cap - 1) { //upgrade capacity if needed
            cap *= RESIZE;
            line = (char *)realloc(line, cap);
        }
        line[size++] = ch; //store char in string
    }

    if(size == 0 && ch == EOF) {
        free(line);
        return NULL;
    }

    line[size] = '\0'; //null terminate end

    return line;
}