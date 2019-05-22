#include "main.h"
#define INPUT_BTS 20


int main() {
  int f;
while (666) {
  puts(
      "ENTER NUMBER OF FUNCTION AND THEN ARGUMENTS\n"
      "0 - Str form(const char*,...);\n"
      "1 - int Strcmp(const char*,...);\n"
      "2 - Str concatStr(Str,Str);\n"
      "3 - Str subs(Str,int,int);\n"
      "4 - Str* splitAt(Str,int);\n"
      "5 - Str Strchr(Str,char);\n"
      "6 - Str Strrchr(Str,char);\n"
  );
  scanf("%d", &f);
  switch (f) {
    case 0:
    do {
     char* format = (char*) malloc(INPUT_BTS*sizeof(char));
     scanf("%s",format);
     Str str1 = scanStr();
     Str str2 = scanStr();
     printStr(form(format,str1,&str2));
     putchar('\n');
    } while(0);
    break;
    case 1:
    do {
     char* format = (char*) malloc(INPUT_BTS*sizeof(char));
     scanf("%s",format);
     Str str1 = scanStr();
     char* s2 = (char*) malloc(INPUT_BTS*sizeof(char));
     scanf("%s",s2);
     printf("%d\n", Strcmp(format,&str1,s2));
    } while(0);
    break;
    case 2:
    do {
     Str str1 = scanStr();
     Str str2 = scanStr();
     printStr(concatStr(str1,str2));
     putchar('\n');
    } while(0);
    break;
    case 3:
    do {
     Str str1 = scanStr();
     int v1;
     scanf("%d",&v1);
     int v2;
     scanf("%d",&v2);
     printStr(subs(str1,v1,v2));
     putchar('\n');
    } while(0);
    break;
    case 4:
    do {
     Str str1 = scanStr();
     int v1;
     scanf("%d",&v1);
     printStr(*(splitAt(str1,v1)));
    } while(0);
    break;
    case 5:
    do {
     Str str1 = scanStr();
     char c1 = getchar();
     printStr(Strchr(str1,c1));
     putchar('\n');
    } while(0);
    break;
    case 6:
    do {
     Str str1 = scanStr();
     char c1 = getchar();
     printStr(Strrchr(str1,c1));
     putchar('\n');
    } while(0);
    break;
  }}
}