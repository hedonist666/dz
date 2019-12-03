#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#define GREET "This toy is powered by Vsevolod Drozdov"
#define output print_dump()
#define ESC 27
#define SIZE 1488
#define RESET "\x1b[0m"
#define WB "\x1b[47m"
#define MB "\x1b[45m"
#define WIDTH 70
#define HEIGHT 10
char* buf;
int n, pos, pp,m=2, start;
char* used;
int min(int a,int b) {
    if (a>b) return b;
    return a;
}
int max(int a,int b) {
  if (a>b) return a;
  return b;
}
void trim() {
  if (buf[n-1]==0xa) {
    buf[n-1]='\0';
    n-=1;
  }
}
void add(char *x) {
  int v=n+strlen(x)+1;
//realloc(buf,v);
  buf[n]=' ';
  for (int i=n;i<v;++i) {
    buf[i+1] = x[i - n];
  }
  n=v;
  buf[n]='\0';
}
void mistake() {
  printf("\a");
}
void move(int x) {
  pos=x;
  if (pos>=n) pos=n-1;
  if (pos<0) pos=0;
}
void insert(char *x) {
  int v=strlen(x)+pos;
  for (int i=pos;i<v;++i) {
      buf[i] = x[i - pos];
  }
  if (v>n) buf[v]='\0';
  pos=v;
}
void put(char* x) {
  int v=n+strlen(x);
//realloc(buf, v);
  for (int i=n-1;i>=pos;--i) {
    buf[i+v-n]=buf[i];
  }
  for (int i=pos;i<pos+v-n;++i) {
    buf[i]=x[i-pos];
  }
  n=v;
  buf[n]='\0';
//pos+=strlen(x);
}
void print_dump() {
  for (int i=0;i<strlen(buf);++i) 
    printf("%d:%c,",i,buf[i]);
  putchar('\n');
}
/*1 - border mode
 *2 - center mode
 *3 - beyond mode*/
void print_screen(int w, int h){ 
  for (int i=0;i<w;++i) {
    putchar('@');
  }
  puts("\n");
  trim();
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
  if (start+sz>n) {
      for (;i<start+sz;++i) {
        putchar('~');
        if (!((i+1)%w)) putchar('\n');
      } 
  }
  puts("");
  for (int i=0;i<w;++i) {
    putchar('@');
  }
  puts("");
}
void save(char *s) {
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
void load(char* s) {
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
void remove_vi(int x, int y) {
  if (x>n) mistake();
  if (y>n) {
    n=x;
    //realloc(buf, n);
  }
  else {
    int b=y-x;
    for (int i=y;i<n;++i) {
        buf[i-b]=buf[i];
    }
    //realloc(buf,n-blya);
    n-=b;
  }
  buf[n]='\0';
  //pos=min(y,n);
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
void upcase() {
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
void lowcase() {
  int p=pos;
  if (buf[pos]==' ')  {mistake();return;}
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
void find(char *x) {
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
void handle_input() {
  struct termios tty,stty;
  char c;
  printf("\ncommand: ");
  fflush(stdout);
  tcgetattr(0, &tty);
  stty=tty;
  tty.c_lflag &= ~(ISIG | ICANON);
  tty.c_cc[VMIN] = 1;
  tcsetattr(0, TCSAFLUSH, &tty);
	read(0, &c, 1);
	tcsetattr(0, TCSANOW, &stty);
  int tmp;
  char* ar=(char *)malloc(666);
  switch (c) {
    case 'u':
      lowcase();
      break;
    case 'U':
      upcase();
      break;
    case 'm':
      scanf("%d",&tmp);
      move(tmp);
      break;
    case 'p':
      scanf("%s",ar);
      put(ar);
      break;
    case 'x':
      del();
      break;
    case 'h':    
      scanf("%d",&tmp);
      movelw(tmp);
      break;
    case 'l':
      scanf("%d",&tmp);
      moverw(tmp);
      break;
    case 'd':
      scanf("%d",&tmp);
      del_ar(tmp);
      break;
    case 'a':
      scanf("%s",ar);
      add(ar);
      break;
    case 'i':
      scanf("%s",ar);
      insert(ar);
      break;
    case 'w':
      scanf("%s",ar);
      save(ar);
      break;
    case 'o':
      scanf("%s",ar);
      load(ar);
      break;
    case 'f':
      moverw(1);
      break;
    case 'b':
      movelw(1);
      break;
    case 's':
      scanf("%s",ar);
      find(ar);
      break;
    case 'c':
      clear();
      break;
    case 't':
      m=((m+1)%3)+1;
      break;
    case 'j':
      moverw(WIDTH);
      break;
    case 'k':
      movelw(WIDTH);
      break;
    case 'q':
      exit(0);
  }
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
  while(666) {
    pp=pos;
    handle_input();
    system("clear");
    print_screen(WIDTH,HEIGHT);
  } 
}
