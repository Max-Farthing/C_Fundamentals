/**
    @file catalog.c
    @author Max Farthing(mafarthi)
    This file is responsible for reading and managing the parks read in at startup
*/

#include "input.h"
#include "catalog.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/** index for county 2*/
#define COUNTY2 2
/** index for county 3 */
#define COUNTY3 3
/** index for county 5 */
#define COUNTY4 4
/** catalog should be able to read in 3 numbers back to back - id + lat + lon */
#define CAT_NUMS 3
/** Multiplier for converting degrees to radians */
#define DEG_TO_RAD ( M_PI / 180 )
/** Radius of the earth in miles. */
#define EARTH_RADIUS 3959.0
/** keeps up with total distance for trip */
double totalDistance = 0;

bool listHelper(struct Park const *park, char const *str) 
{
    if(str == NULL) {
        return true;
    } else {
        for(int i = 0; i < park->numCounties; i++) {
            if(strcmp(str, park->counties[i]) == 0) {
                return true;
            }
        }
        return false;
    }
}

/**
    Helper method to help print out counties for output
    @param park pointer to a park inside of a catalog list
*/
static void printCounties(struct Park const *park) {
    for(int i = 0; i < park->numCounties; i++) {
        if(i != park->numCounties - 1) {
            printf("%s,", park->counties[i]);
        } else {
            printf("%s", park->counties[i]);
        }
    }
    printf("\n");
}


int compIds(void const *park1, void const *park2) 
{
    struct Park *const *p1 = park1;
    struct Park *const *p2 = park2;
    if((*p1)->id < (*p2)->id) { //park1 should be first
        return -1;
    } else if((*p1)->id > (*p2)->id) { //park2 should be first
        return 1;
    } else {
        return 0; //ids are equal
    }
}


int compNames(void const *park1, void const *park2)
{
    struct Park *const *p1 = park1;
    struct Park *const *p2 = park2;
    int result = strcmp((*p1)->name, (*p2)->name);
    if(result != 0) {
        return result;
    } else {
        int res2 = compIds(park1, park2);
        return res2;
    }
}


double distance(struct Park const *a, struct Park const *b) 
{
    double lon1 = a->lon;
    double lon2 = b->lon;
    double lat1 = a->lat;
    double lat2 = b->lat;
    double v1[] = { cos( lon1 * DEG_TO_RAD ) * cos( lat1 * DEG_TO_RAD ), 
                    sin( lon1 * DEG_TO_RAD ) * cos( lat1 * DEG_TO_RAD ),
                    sin( lat1 * DEG_TO_RAD ) };

    double v2[] = { cos( lon2 * DEG_TO_RAD ) * cos( lat2 * DEG_TO_RAD ),
                    sin( lon2 * DEG_TO_RAD ) * cos( lat2 * DEG_TO_RAD ),
                    sin( lat2 * DEG_TO_RAD ) };

    double dp = 0.0;
    for ( int i = 0; i < sizeof( v1 ) / sizeof( v1[ 0 ] ); i++ )
        dp += v1[ i ] * v2[ i ];
  
    double angle = acos( dp );

    totalDistance += EARTH_RADIUS * angle; //change
    return totalDistance;
}


struct Catalog *makeCatalog()
{
    struct Catalog *catalog = (struct Catalog *)malloc(sizeof(struct Catalog));
    catalog->count = 0;
    catalog->capacity = CAT_START_CAP;
    catalog->list = (struct Park **)malloc(catalog->capacity * sizeof(struct Park *));
    return catalog;
}


void freeCatalog(struct Catalog *catalog)
{
    if(catalog == NULL) {
        return;
    }
    if(catalog->list != NULL) {
        for(int i = 0; i < catalog->count; i++) {
            free(catalog->list[i]);
        }
        free(catalog->list);
    }
        free(catalog);
    
}


void readParks(char const *filename, struct Catalog *catalog)
{
    FILE *fp = fopen(filename, "r");
    if(!fp) {
        freeCatalog(catalog);
        fprintf(stderr, "Can't open file: %s\n", filename);
        exit(1);
    }
    int id;
    double lat;
    double lon;
    char cStore[MAX_COUNTIES][COUNTY_LIMIT];
    char *line2 = NULL;
    int offset;
    char *str;
    int scan = 0;
    while((str = readLine(fp))) {
        int countyNum = 0;
        struct Park *park = (struct Park *)malloc(sizeof(struct Park));
        for (int i = 0; i < MAX_COUNTIES; i++) {
            strcpy(park->counties[i], ""); 
        }
        strcpy(park->name, "");
        scan = sscanf(str, "%d%lf%lf%n", &id, &lat, &lon, &offset);
        if(scan != CAT_NUMS) {
            free(park);
            free(str);
            freeCatalog(catalog);
            fprintf(stderr, "Invalid park file: %s\n", filename);
            fclose(fp);
            exit(1);
        }
        char store[MAX_COUNTIES];
        countyNum = sscanf(str + offset, "%s%s%s%s%s%s", cStore[0], cStore[1], cStore[COUNTY2], cStore[COUNTY3], cStore[COUNTY4], store);
        if(countyNum > MAX_COUNTIES) {
            free(park);
            free(str);
            freeCatalog(catalog);
            fprintf(stderr, "Invalid park file: %s\n", filename);
            fclose(fp);
            exit(1);
        }
        line2 = readLine(fp);
        if(!line2) {
            //print error
        }
        park->id = id;
        for(int i = 0; i < catalog->count; i++) {
            if(park->id == catalog->list[i]->id){
                free(park);
                free(str);
                free(line2);
                freeCatalog(catalog);
                fprintf(stderr, "Invalid park file: %s\n", filename);
                fclose(fp);
                exit(1);
            }
        }
        park->lat = lat;
        park->lon = lon;
        park->numCounties = countyNum;
        for(int i = 0; i < countyNum; i++) {
            strcpy(park->counties[i], cStore[i]);
        }
        strcpy(park->name, line2); 
        if(strlen(park->name) > LINE_LIMIT) {
            free(park);
            free(str);
            free(line2);
            freeCatalog(catalog);
            fprintf(stderr, "Invalid park file: %s\n", filename);
            fclose(fp);
            exit(1);
        }

        catalog->list[catalog->count] = park;
        catalog->count++;
        if(catalog->count >= catalog->capacity){ //resizing array if count meets or exceeds capacity
            catalog->capacity *= RESIZE;
            struct Park **newList = (struct Park **)realloc(catalog->list, catalog->capacity * sizeof(struct Park *));
            catalog->list = newList;
        }
        free(str);
        free(line2);
    } 
    


    fclose(fp);
}


void sortParks(struct Catalog *catalog, int (* compare) (void const *va, void const *vb))//javadoc may need change
{
    qsort(catalog->list, catalog->count, sizeof(struct Park*), compare);
}


void listParks(struct Catalog *catalog, bool (* test) (struct Park const *park, char const *str), char const *str)//javadoc may need change
{ 
    if(str == NULL){
        for(int i = 0; i < catalog->count; i++) { 
        struct Park *p = catalog->list[i];
        printf("%-4d%-40s%9.3f%9.3f ", p->id, p->name, p->lat, p->lon);
        printCounties(p);
        }
    } else if (strcmp(str, "trip") == 0){
            for(int i = 0; i < catalog->count; i++) {
                struct Park *p = catalog->list[i];
                    if(i == 0) {
                        printf("%-4d%-40s      0.0\n", p->id, p->name);
                    } else {
                        printf("%-4d%-40s%9.1f\n", p->id, p->name, distance(catalog->list[i], catalog->list[i - 1]));
                    }
            }
            totalDistance = 0;
    } else {
            for(int i = 0; i < catalog->count; i++) {
                struct Park *p = catalog->list[i];
                if(test(p, str)) {
                    printf("%-4d%-40s%9.3f%9.3f ", p->id, p->name, p->lat, p->lon);
                    printCounties(p);
                }
            }
    }
    
}