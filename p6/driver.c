/**
    @file driver.c
    @author Max Farthing (mafarthi)
    This component is the top level main component. Ties program together
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "input.h"
#include "value.h"
#include "map.h"

/** the designated map length */
#define MAP_LEN 100
/** max chars allowed in user input */
#define MAX_INPUT 20

/**
    Main method to tie the program together and to interact with the user via command line
    @return int based upon exit status
*/
int main() 
{

    printf("cmd> ");
    Map *m = makeMap(MAP_LEN);

    char *str;
    while((str = readLine(stdin))) {
        int buffer = 0;
        int buff2 = 0;
        char str2[MAX_INPUT] = "";
        char str3[MAX_INPUT] = "";
        sscanf(str, "%s%n", str2, &buffer);
        int scan = 0;
        scan = sscanf(str + buffer, "%s%n", str3, &buff2);

        if(strcmp(str2, "set") == 0) {
            char str4[MAX_INPUT] = ""; 
            char str5[MAX_INPUT] = "";
            int num = sscanf(str + buff2 + buffer, "%s%s", str4, str5);
            printf("%s %s %s", str2, str3, str4);
            if(num != 1) {
                printf(" %s\n", str5);
                printf("Invalid Command\n");
                printf("\n");
                printf("cmd> ");
                free(str);
                continue;
            }
            printf("\n");
            Value *k;
            Value *v;
            k = (Value *)malloc(sizeof(Value));
            v = (Value *)malloc(sizeof(Value));
            int check = 0;
            check = parseInteger(k, str3);
            if(check == 0) {
                parseString(k, str + buffer);
            }
            check = 0;
                check = parseInteger(v, str4);
            if(check == 0) {
                parseString(v, str4);
            }
            mapSet(m, k, v);
            free(k);
            free(v);

        } else if(strcmp(str2, "get") == 0) { //need to add check for multiple params passed, also key not existing
            char str4[MAX_INPUT] = "";
            sscanf(str + buff2 + buffer, "%s", str4); //might have to add back for error checking
            if(strlen(str4) != 0) {
                printf("%s %s %s\n", str2, str3, str4);
                printf("Invalid command\n");
                printf("\n");
                printf("cmd> ");
                free(str);
                continue;
            }
            bool test = false;
            printf("%s ", str2); 
            Value *k; 
            k = (Value *)malloc(sizeof(Value));
            int check = 0;
            check = parseInteger(k, str3);
            if (check == 0) {
                parseString(k, str3);
                test = true;
            }
            printf("%s\n", str3);
            Value *v = mapGet(m, k);
            if(!v) {
                printf("Undefined\n");
                printf("\n");
                printf("cmd> ");
                if(test) {
                    free(k->vptr);
                }
                free(k);
                free(str);
                continue;
            }
            check = 0;
            v->print(v);
            printf("\n");
            if(test) {
                free(k->vptr);
            }
            free(k);
            
        } else if(strcmp(str2, "remove") == 0) {
            if(strlen(str3) == 0) {
                printf("%s\n", str2);
                printf("Invalid command\n");
                printf("\n");
                printf("cmd> ");
                free(str);
                continue;
            }
            printf("%s %s\n", str2, str3);
            Value *k;
            k = (Value *)malloc(sizeof(Value));
            int check = 0;
            parseInteger(k, str3);
            if (check == 0) {
                parseString(k, str3);
            }
            if(!mapRemove(m, k)) {
                if(check == 0) {
                    free(k->vptr);
                }
                free(k);
                printf("Invalid command\n");
                printf("\n");
                printf("cmd> ");
                free(str);
                continue;
            }
            // mapRemove(m, k);
            free(k);
            
        } else if(strcmp(str2, "size") == 0) { //add check for extra strings
            if(scan == 1) {
                printf("%s %s\n", str2, str3);
                printf("Invalid command\n");
            } else {
                printf("%s\n", str2);
                printf("%d\n", mapSize(m));
            }
            
        } else if(strcmp(str2, "quit") == 0) {
            printf("%s\n", str2);
            free(str);
            freeMap(m);
            return EXIT_SUCCESS;
        } else {
            printf("%s\n", str2);
            printf("Invalid command\n");
        }
        free(str);
        printf("\n");
        printf("cmd> ");
    }

    freeMap(m);
    return EXIT_SUCCESS;
}