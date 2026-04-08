#ifndef HASHMAP_H
#define HASHMAP_H

#include "linkedlist.h"

unsigned int hash(char *str);
Node **hashmap_start();
void hashmap_insert(Node **hashmap, char *str);
int hashmap_retrieve(Node **hashmap, char *str);

#endif
