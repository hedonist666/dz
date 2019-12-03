#include <stdio.h>
#include <stdlib.h>


typedef struct Node {
  int val;
  struct Node* next;
  struct Node* prev;
} Node;

typedef struct List {
  int size;
  Node* head;
  Node* cur;
} List;

List* scanList(FILE*);
List** domath(List*,int);


void showList(List*);
void freeList(List*);
