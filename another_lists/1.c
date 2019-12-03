#include <stdio.h>
#include <stdlib.h>


typedef struct Node {
  int val;
  struct Node* prev;
  struct Node* next;
} Node;

typedef struct List {
  Node* head;
  Node* cur;
} List;


void printList(List s) {
  for (s.cur = s.head; s.cur != NULL; s.cur = s.cur->next) {
    printf("%p: %d (prev: %p)\n", s.cur, s.cur->val, s.cur->prev);
  }
}


void destruct(List s) {
  for (s.cur = s.head; s.cur != NULL;) {
    Node* tmp = s.cur->next;
    free(s.cur);
    s.cur = tmp;
  }
}


List domath(List s) {

  List res;
  s.cur = s.head;
  res.head = malloc(sizeof(Node));
  res.cur = res.head;
  Node* remainder = NULL;
   
  for (int i = 1; s.cur != NULL; ++i, s.cur = s.cur->next) {

    res.cur->val = s.cur->val;
 
    if (res.cur->val >= 0) {
      remainder = res.cur;
    }

    if ( !(i&1) ) {
      if (s.cur->val < 0) continue;
      if (s.cur->val > 0) {
        res.cur->prev      = remainder;
        res.cur->next      = malloc(sizeof(Node));
        res.cur->next->val = res.cur->val;
        res.cur            = res.cur->next;
        if (res.cur->val >= 0) remainder = res.cur;
      }
    }



    res.cur->prev = remainder;

    res.cur->next = malloc(sizeof(Node));

    res.cur->next->prev = res.cur;
    res.cur = res.cur->next;
  }
  
  res.cur = res.cur->prev;
  free(res.cur->next);
  res.cur->next = NULL;

  return res;

}


int main() {
  int n;
  List s;
  scanf("%d", &n);
  if (!n) {
    puts("done");
    return 0;
  }
  s.head = malloc(sizeof(Node));
  s.cur = s.head;
  s.cur->prev = NULL;
  for (int i = 0; i < n - 1; ++i, s.cur = s.cur->next) {
    scanf("%d", &s.cur->val);
    s.cur->next = malloc(sizeof(Node));
    s.cur->next->prev = s.cur;
  }
  scanf("%d", &s.cur->val);
  s.cur->next = NULL;
    
  printList(s);

  puts("===============");

  List s1 = domath(s);
  printList(s1);

  destruct(s);
  destruct(s1);
  
}
