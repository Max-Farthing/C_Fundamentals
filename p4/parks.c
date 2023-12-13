/**
    @file parks.c
    @author Max Farthing(mafarthi)
    This file is the top level component of the program containing the main method to interact with the user
*/

#include "input.h"
#include "catalog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <stdbool.h>

/** the limited amount of chars allowed for a command */
#define COMMAND_LIMIT 7
/** amount of characters allowed for an error string */
#define ERROR_STRING_LIM 10

/**
    Main function responsible for reading from input stream and in this program ties all other components together
    for the final output
    @param argc number of line arguments provided
    @param argv array of char pointers used for file reading
    @return int value for successful or non-successful program output
*/
int main(int argc, char *argv[])
{
    if(argc <= 1) {
        fprintf(stderr, "usage: parks <park-file>*\n");
        return 1;
    }
    
    struct Catalog *c1  = makeCatalog(); //catalog of all parks

    for(int i = 1; i < argc; i++) {
        readParks(argv[i], c1);
    }

    struct Catalog *c2 = makeCatalog(); //blank catalog to be used as trip catalog
    printf("cmd> ");
    char str1[COMMAND_LIMIT], str2[COMMAND_LIMIT], str3[COUNTY_LIMIT];
    int n;
    int y;
    int lookUpId;

    char *str;
    while((str = readLine(stdin))) { //capturing user input in a loop for user to interact with program via stdin
        sscanf(str, "%6s%n", str1, &n);
       

        if(strcmp(str1, "list") == 0){ //check if user wants program to list something or do another action
        sscanf(str + n, "%s%n", str2, &y);
        


            if(strcmp(str2, "parks") == 0) { //list all available parks
                printf("%s %s\n", str1, str2);
                printf("ID  Name                                          Lat      Lon Counties\n");
                sortParks(c1, compIds); //sort by id
                listParks(c1, listHelper, NULL); //list functions

            } else if(strcmp(str2, "names") == 0) { //list all parks sorted by name
                printf("list names\n");
                printf("ID  Name                                          Lat      Lon Counties\n"); 
                sortParks(c1, compNames);
                listParks(c1, listHelper, NULL);

            } else if(strcmp(str2, "county") == 0) { //list parks in catalog within county specified
                sscanf(str + y + n, "%s", str3);
                printf("%s %s %s\n", str1, str2, str3);
                printf("ID  Name                                          Lat      Lon Counties\n");

                sortParks(c1, compNames);
                listParks(c1, listHelper, str3);
            }
        } else if(strcmp(str1, "add") == 0){ //add park to trip list
            sscanf(str + n, "%d", &lookUpId);
            if(lookUpId == -1) { //if id == -1 means an int was not read in therefore read in string
                char store[ERROR_STRING_LIM] = "";
                sscanf(str + n, "%s", store);
                printf("%s %s\n", str1, store);
                printf("Invalid command\n");
            } else {
                bool check = false;
                printf("%s %d\n", str1, lookUpId);

                if(c2->count >= c2->capacity) { //resizing trips list if neccessary
                    c2->capacity *= RESIZE;
                    c2->list = (struct Park**)realloc(c2->list, c2->capacity * sizeof(struct Park *));
                }

                for(int i = 0; i < c1->count; i++) {
                    if(c1->list[i]->id == lookUpId){
                        c2->list[c2->count] = c1->list[i]; //adding park to end of trip catalog
                        check = true;
                        c2->count++;
                        break;
                    }
                }

                if(!check) {
                    printf("Invalid command\n"); //call to add but ID is not in parks catalog == error
                }
            }
            
            lookUpId = -1; //set id to -1 for invalid read in check
            
        } else if(strcmp(str1, "remove") == 0) { //remove from trip
            bool remove = false;
            bool once = false;
            sscanf(str + n, "%d", &lookUpId);
            printf("%s %d\n", str1, lookUpId);
            for(int i = 0; i < c2->count; i++) {
                if(lookUpId == c2->list[i]->id && !once) { //find if id exists in trip catalog
                    once = true;
                    if( i != 0 ) {
                        totalDistance -= distance(c2->list[i], c2->list[i - 1]);
                    }
                    for(int j = i; j < c2->count - 1; j++){
                        c2->list[j] = c2->list[j + 1]; //if it exists shift entire list of catalog
                    }
                    c2->list[c2->count - 1] = NULL; //set last element null to account for shift
                    remove = true;
                    c2->count--;
                    i--; //back up iteration by 1 to make sure we are checking all elements
                }
            }
            if(!remove) {
                printf("Invalid command\n"); //call to remove but ID is not in trip == error thrown
            }


        } else if(strcmp(str1, "trip") == 0) { //list trip
            printf("%s\n", str1);
            printf("ID  Name                                     Distance\n"); 
            listParks(c2, listHelper, str1);

        } else if(strcmp(str1, "quit") == 0){ //quit program
            
            printf("%s\n", str1);
            free(str);
            free(c2->list);
            free(c2);
            freeCatalog(c1);
            return EXIT_SUCCESS;
        } else {
            printf("%s\n", str1); //invalid command has been thrown
            printf("Invalid command\n");
        }
        free(str);
        printf("\n");
        printf("cmd> "); //reprompting user
    }
    
    free(c2->list);
    free(c2);
    freeCatalog(c1); //freeing memory of catalogs and their lists/parks
    return EXIT_SUCCESS;
}