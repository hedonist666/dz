#include <stdio.h>
#include <stdlib.h>


typedef struct Node {
  int val;
  struct Node* next;
  struct Node* prev;
} Node;


typedef struct List {
  Node* head;
  Node* cur;
} List;


int compare(List a, List b) {
  b.cur = b.head;
  int i;
  for (i = 0; b.cur != NULL; a.cur = a.cur->next, b.cur = b.cur->next, ++i) {
    if (a.cur == NULL) return 0;
    if (a.cur->val != b.cur->val) return 0; 
  }
  return i;
}

List remv(List s, int n) {
  Node* pr = s.cur->prev;
  for (int i = 0; i < n; ++i) {
    Node* tmp = s.cur->next;
    free(s.cur);
    s.cur = tmp;
  }
  if (pr) pr->next = s.cur;
  if (s.cur) s.cur->prev = pr;
  return s;
}

List subtract(List a, List b) {
  int len;
  a.cur = a.head;
  if ((len = compare(a, b))) {
    a = remv(a, len);
    a.head = a.cur;
    return a;
  }
  a.cur = a.cur->next;
  for (; a.cur != NULL; a.cur = a.cur->next) {
    if ((len = compare(a, b))) {
      a = remv(a, len);
      return a;
    }
  }
  return a;
}

List domath(List s, int n, List* a) {
  for (int i = 0; i < n; ++i) {
    s = subtract(s, a[i]);
  }
  return s;
}

List scanList() {
  int n;
  List s;
  scanf("%d", &n);
  if (!n) {
    puts("done");
    exit(0);
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
  return s;
} 

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

int main() {
  List x = scanList();
  int n;
  scanf("%d", &n);
  List* a = malloc(n * sizeof(List));
  for (int i = 0; i < n; ++i) {
    a[i] = scanList();
  }
  x = domath(x, n, a);
  printList(x);
  destruct(x);
  for (int i = 0; i < n; ++i) {
    destruct(a[i]);
  }
  free(a);
}
