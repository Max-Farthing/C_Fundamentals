/**
    @file catalog.h
    @author Max Farthing(mafarthi)
    This header file allows catalog to work with other files in the program
*/

#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/** max allowed length of county name */
#define COUNTY_LIMIT 13
/** max allowed number of counties per park */
#define MAX_COUNTIES 5
/** limit on allowed length of line */
#define LINE_LIMIT 40
/** Capacity of catalog when first created */
#define CAT_START_CAP 5

/** global variable to keep track of total trip distance */
extern double totalDistance;

/** Park struct containing info on park details */
struct Park {
    int id;
    char name[LINE_LIMIT];
    double lat;
    double lon;
    int numCounties;
    char counties[MAX_COUNTIES][COUNTY_LIMIT];
};

/** Catalog struct containing info on catalog details */
struct Catalog {
    struct Park **list;
    int count; 
    int capacity;
};

/**
    This function is responsible for returning the distance between 2 parks in miles.
    @param a A park struct that is const
    @param b A park struct that is const
    @return double amount of miles between the 2 parks
*/
double distance( struct Park const *a, struct Park const *b);

/**
    This function dynamically allocates storage for the given catalog, initializes catalog fields
    and returns pointer to new catalog(like a java constructor)
    @return pointer to new catalog
*/
struct Catalog *makeCatalog();

/**
    Frees memory used to store given catalog including: freeing space for all parks, freeing resizeable array of pointers
    frees catalog struct itself
    @param catalog The catalog to be freed
*/
void freeCatalog(struct Catalog *catalog);

/**
    Reads all parks from a park file with provided filename. Makes instance of park struct for each park read in and stores a pointer
    to each park in resizeable array in catalog
    @param filename Name of file provided to be used for IO
    @param catalog the catalog where park information pointers will be stored
*/
void readParks(char const *filename, struct Catalog *catalog);

/**
    Sorts parks in given catalog, uses qsort()
    @param catalog Catalog to be sorted
    @param compare qsort function pointer
    @param va const value 1
    @param vb const value 2
*/
void sortParks(struct Catalog *catalog, int (* compare) (void const *va, void const *vb));

/**
    Prints all or some of parks upon request
    @param catalog Catalog to be printed from
    @param test function pointer to be used
    @param str String to be used for printing
*/
void listParks(struct Catalog *catalog, bool (*test) (struct Park const *park, char const *str), char const *str);

/**
    Compare parks using their IDS only
    @param park1 void pointer to park 1
    @param park2 void pointer to park 2
    @return int returns int based upon what order they should return in
*/
int compIds(void const *park1, void const *park2);

/**
    Compares parks using names first, if names are the same then compares using IDs
    @param park1 void pointer to park1
    @param park2 void pointer to park2
    @return int based upon the order the parks should be sorted
*/
int compNames(void const *park1, void const *park2);

/**
    Helper method for ListParks to help determine if a park should be printed or not
    @param park pointer to a park
    @param str pointer to a string
    @return boolean whether the park should be printed out or not
*/
bool listHelper(struct Park const *park, char const *str);
