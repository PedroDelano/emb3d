/*
 *
 * A simple linked list implementation
 * to be used for hash maps
 *
 * */

#include "linkedlist.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *create_node() {
  // we need to allocate memory for
  // the struct to be able to use it
  // outside the function.
  Node *head = malloc(sizeof(Node));
  head->next = NULL;
  return head;
}

Node *insert_ll(Node *head, Node *n) {
  // to keep inserts O(1) and considering
  // we can ignore order, we can just append
  // the new element to the start of the LL
  assert(n->next == NULL && "cannot insert a linked node at the start.");

  // promote it to head
  n->next = head;
  return n;
}

int search_ll(Node *head, char *str) {
  // we are trying to find the str in Data
  Node *curr = head;
  while (curr) {
    if (strcmp(curr->data.str, str) == 0) {
      return curr->data.idx;
    }
    curr = curr->next;
  }
  printf("ERROR: str not found\n");
  return -1;
}
