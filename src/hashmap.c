/**********************************
 *
 * A simple hashmap implementation
 * using our implementation of
 * linked lists to handle collisions
 *
 **********************************/

#include "linkedlist.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The number of available slots to fill
// the map. After 1024 it will reuse hashes
// and we need to use linked list search
const int SLOT_SIZE = 1024;

// The MULTIPLIER should be a prime number
// so that we avoid dividing the SLOT_SIZE
// and reducing it.
const int MULTIPLIER = 13;

unsigned int hash(char *str) {
  unsigned int hash = 0;
  for (size_t i = 0; i < strlen(str); i++) {
    hash = hash * MULTIPLIER + str[i];
  }
  return hash % SLOT_SIZE;
}

Node **hashmap_start() {

  // Allocate an array of SLOT_SIZE pointers to Nodes
  Node **hashmap = malloc(SLOT_SIZE * sizeof(Node *));

  // Initialize values
  for (int i = 0; i < SLOT_SIZE; i++) {
    hashmap[i] = NULL;
  }

  return hashmap;
}

void hashmap_insert(Node **hashmap, char *str) {

  // retrieving the index from a given string
  int index = hash(str);
  assert(index < SLOT_SIZE && "index is greater than SLOT_SIZE");

  // creating the linked list node to be inserted
  Node *n = create_node();
  struct Data d = {str, index};
  n->data = d;

  // inserting it
  Node *head = insert_ll(hashmap[index], n);
  hashmap[index] = head;
}

int hashmap_retrieve(Node **hashmap, char *str) {
  // retrieving the index from a given string
  int index = hash(str);
  assert(index < SLOT_SIZE && "index is greater than SLOT_SIZE");
  return search_ll(hashmap[index], str);
}
