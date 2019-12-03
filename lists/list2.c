#include "list2.h"

List* scanList(FILE* f) {
  int n;
  fscanf(f, "%d", &n);
  List* res = (List*) malloc(sizeof(List) * n);
  res->size = n;
  res->head = (Node*) malloc(sizeof(Node));
  res->cur = res->head;
  for (int i = 0; i < n - 1; ++i, res->cur = res->cur->next) {
    fscanf(f, "%d", &res->cur->val);
    res->cur->next = (Node*) malloc(sizeof(Node));
    res->cur->next->prev = res->cur;
  }
  
  fscanf(f, "%d", &res->cur->val);
  res->cur->next = res->head;
  res->head->prev = res->cur;

  return res;
}

void showList(List* lst) {
  puts("====================");
  lst->cur = lst->head;
  do {
    printf("%p: %d\n", lst->cur, lst->cur->val);
    lst->cur = lst->cur->next;
  } while (lst->cur != lst->head);
  puts("====================");
}

void freeList(List* lst) {
  lst->cur = lst->head;
  do {
    Node* tmp = lst->cur->next;
    free(lst->cur);
    lst->cur = tmp;
  } while (lst->cur != lst->head);
  free(lst);
}


List** domath(List* lst, int n) {
  if (n > lst->size) {
    puts("N > size of list, dunno what to do");
    exit(0);
  }

  lst->cur = lst->head;

  List** res = (List**) malloc(n * sizeof(List*));

  for (int i = 0; i < n; ++i, lst->cur = lst->cur->next) {
    res[i] = (List*) malloc(sizeof(List));
    res[i]->head = lst->cur; 
  }

  lst->cur = lst->head;

  int* cnt = (int*) malloc(n * sizeof(int));

  for (int i = 0; i < lst->size % n; ++i) {
    cnt[i] = lst->size/n + 1; 
  }
  for (int i = lst->size % n; i < n; ++i) {
    cnt[i] = lst->size/n;
  }

  lst->cur = lst->head;

  for (int i = 0; i < lst->size; ++i) {
    Node* tmp = lst->cur->next;
    if (--cnt[i % n]) {
      Node* sled = lst->cur; 
      for (int j = 0; j < n; ++j, sled = sled->next ) ;
      lst->cur->next = sled;
      sled->prev = lst->cur;
      lst->cur = tmp;
    }
    else {
      lst->cur->next = res[i % n]->head;
      res[i % n]->head->prev = lst->cur;
    }
    lst->cur = tmp;
  }

  free(cnt);
  return res;
}

void main() {
  FILE* f = fopen("sample" , "r");
  List* lst = scanList(f);       
  int n;
  fscanf(f, "%d", &n);
  List** res = domath(lst, n);
  for (int i = 0; i < n; ++i) {
    showList(res[i]);
    freeList(res[i]);
  }
  free(res);
  free(lst);
  fclose(f);
}
