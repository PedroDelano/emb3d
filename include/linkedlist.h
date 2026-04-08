#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct Data {
  char *str;
  int idx;
} Data;

typedef struct Node {
  Data data;
  struct Node *next;
} Node;

Node *create_ll();
Node *insert_ll(Node *head, Node *n);
int search_ll(Node *head, char *str);

#endif
