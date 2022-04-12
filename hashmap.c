#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {

    long posicion = hash(key, map->capacity);

    while ((map->buckets[posicion]!=NULL)&&(map->buckets[posicion]->key!=NULL))
    {
        posicion++;

        if(posicion==(map->capacity))
            posicion=0;
    }


    Pair * newPair = createPair(key, value);
    map->buckets[posicion]= newPair;

    map->size++;
}

void enlarge(HashMap * map) {

    enlarge_called = 1; //no borrar (testing purposes)

    Pair** old_Buckets = map->buckets;
    long old_Capacity = map->capacity;

    map->capacity *= 2;

    Pair** newBuckets = (Pair**) calloc(map->capacity, sizeof(Pair*));

    map->buckets = newBuckets;

    map->size=0;

    for (size_t i = 0; i < old_Capacity; i++)
    {
        if((old_Buckets[i]!=NULL)&&(old_Buckets[i]->key!=NULL))
            insertMap(map, old_Buckets[i]->key, old_Buckets[i]->value);
    }
}


HashMap * createMap(long capacity) {

    HashMap * map = (HashMap *)malloc(sizeof(HashMap));
    map->buckets = (Pair**) calloc(capacity, sizeof(Pair *));
    map->capacity = capacity;
    map->size = 0;
    map->current = -1;

    return map;    
}

void eraseMap(HashMap * map,  char * key) {

    long posicion = hash(key, map->capacity);

    if (is_equal((map->buckets[posicion]->key),key))
    {
        map->buckets[posicion]->key=NULL;
        map->size--;
    }
    else{
        while ((map->buckets[posicion]!=NULL)&&(map->buckets[posicion]->key!=NULL))
        {
            posicion++;

            if(posicion==(map->capacity))
                posicion=0;

            if(map->buckets[posicion]==NULL)
                break;

            if (is_equal((map->buckets[posicion]->key),key))
                map->buckets[posicion]->key=NULL;

            map->size--;   
        }
    }
}

Pair * searchMap(HashMap * map,  char * key) {   

    long posicion = hash(key, map->capacity);

    if(map->buckets[posicion]==NULL)
        return NULL;

    if (is_equal((map->buckets[posicion]->key),key))
    {
        map->current=posicion;
        return map->buckets[posicion];
    }
    
    while ((map->buckets[posicion]!=NULL)||(map->buckets[posicion]->key!=NULL))
    {
        posicion++;

        if(posicion==(map->capacity))
            posicion=0;

        if(map->buckets[posicion]==NULL)
            return NULL;

        if (is_equal((map->buckets[posicion]->key),key))
        {
            map->current=posicion;
            return map->buckets[posicion];
        }
    }

    return NULL;
}

Pair * firstMap(HashMap * map) {

    long posicion = 0;

    while ((map->buckets[posicion]==NULL)||(map->buckets[posicion]->key==NULL))
    {
        posicion++;

        if(posicion==(map->capacity))
            posicion=0;
    }

    map->current = posicion;
    return map->buckets[posicion];
}

Pair * nextMap(HashMap * map) {

    long next = (map->current)+1;

    if(next==map->capacity)
        next = 0;

    while ((map->buckets[next]==NULL)||(map->buckets[next]->key==NULL))
    {
        next++;

        if(next==(map->capacity))
        {
            next=0;
            return NULL;
        }
    }
    
    map->current=next;
    return map->buckets[next];
}
