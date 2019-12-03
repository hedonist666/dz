#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/*
 * some specs for Str:
 * %S - Str
 * %P - pointer to Str
 * %s - just old char*
 */
typedef struct Str {
  int len;
  char* syms;
} Str;

int printStr(Str); //ignore
Str scanStr(); //ignore

Str init(char*); //ignore
char* tocstr(Str);  //ignore

Str form(const char*,...);
int Strcmp(const char*,...);

Str concatStr(Str,Str);
Str subs(Str,int,int);

int length(Str) __attribute__((const)); //ignore
void setlen(Str*,int); //ignore

Str* splitAt(Str,int);
Str Strchr(Str,char);
Str Strrchr(Str,char);
