#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#define GREET "TEXT REDACTOR BY VITYA LAVRUHIN"
#define ESC 27
#define SIZE 1488
#define RESET "\x1b[0m"
#define WB "\x1b[47m"
#define MB "\x1b[45m"
#define WIDTH 70
#define HEIGHT 10


char* buf;
int n, pos, pp, m=2, start;
char* used;

int min(int a,int b) {
    if (a>b) return b;
    return a;
}

int max(int a,int b) {
  if (a>b) return a;
  return b;
}

void add(char *x) { // добавляет в конец
  int v=n+strlen(x)+1;
  buf[n]=' ';
  for (int i=n;i<v;++i) {
    buf[i+1] = x[i - n];
  }
  n=v;
  buf[n]='\0';
}

void mistake() { // издает звук
  printf("\a");
}

void move(int x) { // важная функция, перемещает курсор с парочокой проверок
  pos=x;
  if (pos>=n) pos=n-1;
  if (pos<0) pos=0;
}

void insert(char *x) { // вставляет с переписыванием
  int v=strlen(x)+pos;
  for (int i=pos;i<v;++i) {
      buf[i] = x[i - pos];
  }
  if (v>n) buf[v]='\0';
  pos=v;
}

void put(char* x) { // вставляет
  int v=n+strlen(x);
  for (int i=n-1;i>=pos;--i) {
    buf[i+v-n]=buf[i];
  }
  for (int i=pos;i<pos+v-n;++i) {
    buf[i]=x[i-pos];
  }
  n=v;
  buf[n]='\0';
}

/*1 - border mode
 *2 - center mode
 *3 - beyond mode*/

void print_screen(int w, int h){  // печает все на экран
  int sz=h*w;
  int i;
  if (sz>n) {
    start=0;
  }
  else if (start>pos) {
    start-=w;
  }
  else if (m==1) {
    if (start+sz-w<pos) start+=w;
  }
  else if (m==2) {
    start=(pos-(sz/2))/w*w;
  }
  else if (m==3) {
    if (start+sz<pos) start+=w;
  }
  else {
    perror("uknown mode\n");
    exit(0);
  }
  if (start<0) start=0;
  for (i=start;i<min(start+sz,n);++i) {
   if (buf [i] == '\n') {
     putchar('\n');
     continue;
   }
   if (i==pos) {
      printf(WB "%c" RESET,buf[i]); 
    }
   else if (used[i]==2) {
      printf(MB "%c" RESET, buf[i]);
    }
    else {
      putchar(buf[i]);
    }
    if (!((i+1)%w)) putchar('\n');
  }
}

void save(char *s) { // сохраняет файл
  FILE* f;
  f=fopen(s, "w");
  if (f) {
    fputs(buf,f);
    fclose(f);
  }
  else {
    perror("chtoto ne tak s filom mda..\n");
    exit(-1);
  }
}

void load(char* s) { // загружает файл
  FILE* f;
  f=fopen(s,"r");
  if (f) {
    fseek(f,0,SEEK_END);
    int sz=ftell(f);
    if (sz<=SIZE) {
      fseek(f,0,SEEK_SET);
      fread(buf,sz,1,f);
      fclose(f);
      buf[sz]='\0';
      n=sz;
    }
    else {
      mistake();
    }
  } 
  else {
    perror("chtoto ne tak s filom mda..\n");
    exit(-1);
  }
}

void remove_vi(int x, int y) { // удаление
  if (x>n) mistake();
  if (y>n) {
    n=x;
  }
  else {
    int b=y-x;
    for (int i=y;i<n;++i) {
        buf[i-b]=buf[i];
    }
    n-=b;
  }
  buf[n]='\0';
}

void del() {
  remove_vi(pos,pos+1);
}
void del_ar(int x) {
  remove_vi(pos,pos+x);
}
void movelw(int x) {
  move(pos-x);
}
void moverw(int x) {
  move(pos+x);
}

void upcase() { // как функция внизу только в верхний
  int p=pos;
  if (buf[pos]==' ') {mistake();return;}
  while (buf[pos]!=' ' && pos>=0) {
    pos-=1;
  }
  pos+=1;
  while (buf[pos]!=' ' && pos<n) {
    if (buf[pos] >= 'a' && buf[pos]<='z') {
      buf[pos]+='A'-'a';
    }
    pos+=1;
  }
  pos=p;
}

void lowcase() { // переводит слово, на котором курсор, в нижний курсор
  int p=pos;
  if (buf[pos]==' ')  {
    mistake();
    return;
  }
  while (buf[pos]!=' ' && pos>=0) {
    pos-=1;
  }
  pos+=1;
  while (buf[pos]!=' ' && pos<n) {
    if (buf[pos] >= 'A' && buf[pos]<='Z') {
      buf[pos]-='A'-'a';
    }
    pos+=1;
  }
  pos=p;
}

void find(char *x) { // подсвечивает найденные символы
  int ln=strlen(x);
  int j;
  char flag=0;
  for (int i=0;i<=n-ln;++i) {
    if (buf[i]==x[0]) {
      for (j=i;j<i+ln;++j) 
        if (buf[j]!=x[j-i]) break;
      if (j==i+ln) {
        flag=1;
        for (j=i;j<i+ln;++j) used[j]=2;
      }
    }
  }
  if (!flag) mistake();
}
void clear() {
  for (int i=0;i<n;++i)
    if (used[i]==2)
      used[i]=0;
}

int main(int argc, char** argv) {
  buf=(char *)malloc(SIZE);
  used=(char *)malloc(SIZE);
  if (argc>1) {
    load(argv[1]);
  }
  else strcpy(buf, GREET);
  n=strlen(buf);
  pos=0;
  start=0;
  print_screen(WIDTH,HEIGHT);


  //напиши тут парочку функция для юзания
  free(buf);
  free(used);

}
