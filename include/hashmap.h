#ifndef HASHMAP_H
#define HASHMAP_H

#include "linkedlist.h"

extern const int SLOT_SIZE;

unsigned int hash(char *str);
Node **hashmap_start();
void hashmap_insert(Node **hashmap, char *str, int idx);
int hashmap_retrieve(Node **hashmap, char *str);

#endif
