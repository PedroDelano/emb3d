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

Node *create_node();
Node *insert_ll(Node *head, Node *n);
int search_ll(Node *head, char *str);
char *search_ll_by_index(Node *head, int idx);

#endif
