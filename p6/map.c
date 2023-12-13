/**
    @file map.c
    @author Max Farthing (mafarthi)
    Implementation for the map component, a hash map.
  */

#include "map.h"
#include <stdlib.h>
#include <stdbool.h>
#include "value.h"

typedef struct MapPairStruct MapPair;

/** Key/Value pair to put in a hash map. */
struct MapPairStruct {
  /** Key part of this node, stored right in the node to improve locality. */
  Value key;
  
  /** Value part of this node, stored right in the node to improve locality. */
  Value val;
  
  /** Pointer to the next node at the same element of this table. */
  MapPair *next;
};

/** Representation of a hash table implementation of a map. */
struct MapStruct {
  /** Table of key / value pairs. */
  MapPair **table;

  /** Length of the table. */
  int tlen;
  
  /** Number of key / value pairs in the map. */
  int size;
};

Map *makeMap(int len)
{
    Map *m = (Map *)malloc(sizeof(Map));
    m->table = (MapPair **)malloc(len * sizeof(MapPair *));
    for(int i = 0; i < len; i++) { 
        m->table[i] = NULL; //initialize all of array elements
    }
    m->tlen = len;
    m->size = 0;
    return m;
}

int mapSize(Map *m) 
{
    return m->size;
}

void mapSet(Map *m, Value *key, Value *val)
{
    MapPair **current = &m->table[key->hash(key) % m->tlen]; //find mapping array bucket
    while(*current != NULL) { //iterate through list contained in array bucket
        if(key->equals(key, &(*current)->key)) {
            (*current)->val.empty(&(*current)->val); //free old value
            (*current)->val.move(val, &(*current)->val);
            return; //return if already exists
        }
        current = &(*current)->next;
    }

    MapPair *newP = (MapPair *)malloc(sizeof(MapPair)); //if does not exist create new mapEntry
    key->move(key, &newP->key);
    val->move(val, &newP->val); //store new entries inside of new entry
    newP->next = NULL;

    *current = newP;
    m->size++;//update size if new entry created
}

Value *mapGet( Map *m, Value *key )
{
    MapPair **current = &m->table[key->hash(key) % m->tlen]; //find mapping array bucket
    while(*current != NULL) { //iterate through list contained in array bucket
        if(key->equals(key, &(*current)->key)) {
            return &(*current)->val;
        }
        current = &(*current)->next;
    }
    return NULL;
}

bool mapRemove( Map *m, Value *key )
{
    MapPair **current = &m->table[key->hash(key) % m->tlen]; //find mapping array bucket
    // MapPair *prev = NULL; //keep track of previous value

    while(*current != NULL) { //iterate through list contained in array bucket
        if(key->equals(key, &(*current)->key)) {
            // if(prev != NULL) { //if prev is not null then set its next to the next of the value being removed
            //     prev->next = (*current)->next;
            // } else {
            //     m->table[key->hash(key) % m->tlen] = (*current)->next;
            // }
            (*current)->val.empty(&(*current)->val); //empty value
            (*current)->key.empty(&(*current)->key); //empty value
            free(*current);
            *current = NULL;
            m->size--;
            return true; //key found
        }
        // prev = *current; //iterating through list
        current = &(*current)->next;
    }

    return false; //key never found
}

void freeMap( Map *m )
{
    for(int i = 0; i < m->tlen; i++) {
        MapPair *current = m->table[i]; //get first array bucket
        while(current != NULL) { //iterate through linked list in array bucket
            MapPair *temp = current; 
            current = current->next;
            temp->key.empty(&temp->key); 
            temp->val.empty(&temp->val);
            free(temp); 
        }
    }
    free(m->table);
    free(m);
}