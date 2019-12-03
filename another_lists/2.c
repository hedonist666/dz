#include <stdio.h>
#include <stdlib.h>


typedef struct Node {
  struct Node* next;
  struct Node* prev;
  int val;
} Node;


typedef struct List {
  Node* head;
  Node* cur;
} List;


int length(List s) {
  int res = 0;
  s.cur = s.head;
  do {
    ++res;
    s.cur = s.cur->next;
  } while (s.cur != s.head);
  return res;
}
List scanList() {
  int n;
  List s;
  scanf("%d", &n);
  if ( n <= 0 ) {
    puts("done");
    exit(0);
  }
  s.head = malloc(sizeof(Node));
  s.cur = s.head;
  for (int i = 0; i < n - 1; ++i, s.cur = s.cur->next) {
    scanf("%d", &s.cur->val);
    s.cur->next = malloc(sizeof(Node));
    s.cur->next->prev = s.cur;
  }
  scanf("%d", &s.cur->val);
  s.cur->next = s.head;
  s.head->prev = s.cur;
  s.cur = s.head;
  return s;
} 


void destruct(List s) {
  s.cur = s.head;
  do {
    Node* tmp = s.cur->next;
    free(s.cur);
    s.cur = tmp;
  } while (s.cur != s.head);
}

Node* step_n(List s, int n) {
  Node* st = s.cur;
  for (int i = 0; i < n; ++i, st = st->next) ;
  return st;
}

List* domath(List s, int n) {

  List* ss = (List*) malloc( n * sizeof(List) );

  for (int i = 0; i < n; ++i, s.cur = s.cur->next) {
    ss[i].head = ss[i].cur = s.cur;
  }


  int len = length(s);
  //int am  = len % n ? len/n + 1 : len/n;
  int am  = len/n;

  List res; 
  s.cur = s.head;

  for (int i = 1; i < am; ++i) {
    for (int j = 0; j < n; ++j) {
      Node* tmp = s.cur->next;
      s.cur->next = step_n(s, n);
      s.cur->next->prev = s.cur;
      s.cur = tmp;
    }
  }

  for (int j = 0; j < n; ++j) {
    Node* tmp = s.cur->next;
    s.cur->next = ss[j].head;
    s.cur = tmp;
  }    

  if (len % n) {
    
  }

  return ss;
}


void printList(List s) {
  s.cur = s.head;
  puts("\n===========================");
  do {
    printf("%p: %d (prev: %p)\n", s.cur, s.cur->val, s.cur->prev);
    s.cur = s.cur->next;
  } while (s.cur != s.head);
  puts("===========================");
}

int main() {

  int n;
  scanf("%d",&n);
  List s = scanList(); 

  List* ss = domath(s, n);

  s.cur = s.head;


  for (int i = 0; i < n; ++i) {
    printf("K: %d\n", i+1);
    printList(ss[i]);
  }


  for (int i = 0; i < n; ++i) {
    destruct(ss[i]);
  }

  free(ss);

}
