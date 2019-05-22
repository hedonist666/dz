#include "unity.h"
#include "main.h"
#include "time.h"
#include "stdbool.h"

#define FORMAT_TEST_AMOUNT 10
#define STRCMP_TEST_AMOUNT 10
#define CSTR_TEST_AMOUNT 10
#define STR_R_CHR_TEST_AMOUNT 10
#define SPLITAT_TEST_AMOUNT 10
#define SUBS_TEST_AMOUNT 10
#define MAX_LEN 1000
#define MIN_LEN 100

void setUP() {
  srand(time(NULL));
}

void tearDown() {

}


bool eqs(Str a,Str b) {
  if (a.len != b.len) return false;
  for (int i = 0; i < a.len; ++i) {
    if (a.syms[i] != b.syms[i]) return false;
  }
  return true;
}

char* genRandStr() {
  int m = (rand() % MAX_LEN) + MIN_LEN;
  char* s = (char*) malloc(m*sizeof(char));
  int i;
  for (i = 0; i < m; ++i) {
    s[i] = rand() % 0xff;
  }
  s[i] = '\0';
  return s;
}
void test_format() {
  for (int i = 0; i < FORMAT_TEST_AMOUNT; ++i) {
    char* s1 = genRandStr();
    char* s2 = genRandStr();
    Str str1 = init ( s1 );
    Str str2 = init ( s2 );
    TEST_ASSERT( eqs(concatStr(str1,str2), form("%S%S",str1,str2))  );
    TEST_ASSERT( eqs(concatStr(str1,str2), form("%s%s",s1,s2))     );
    free(s1);
    free(s2);
    free(str1.syms);
    free(str2.syms);
  }    
}


void test_cstr() {
  for (int i = 0; i < CSTR_TEST_AMOUNT; ++i) {
    char* s1 = genRandStr();
    Str str = init ( s1 );
    TEST_ASSERT_EQUAL_STRING( s1, tocstr(str) );
    TEST_ASSERT_EQUAL_INT( 0, strcmp( s1, tocstr(str) ));
    free(s1);
    free(str.syms);
  }
}


char* csubs(char* s, int fr, int to) {
  if ( fr > to || fr > strlen(s) ) return NULL;
  return strndup(&s[fr], to-fr+1);
}

void test_subs() {
  for (int i = 0; i < SUBS_TEST_AMOUNT; ++i) {
    char* s = genRandStr();
    Str str = init ( s );
    int b = rand() % (strlen(s)+100), a = rand() % (strlen(s)+100);
    char* sbs = csubs(s,a,b);
    Str sbstr = subs(str,a,b);
    if (sbs == NULL) {
      TEST_ASSERT_EQUAL_INT( NULL, sbstr.syms);
    }
    else {
      TEST_ASSERT_EQUAL_STRING( sbs, tocstr(sbstr)  );
      TEST_ASSERT( eqs( sbstr, init( sbs ) )  );
    }
    free(s);
    free(str.syms);
    free(sbs);
    free(sbstr.syms);
  } 
}



void test_splitat() {
  for (int i = 0; i < SPLITAT_TEST_AMOUNT; ++i) {
    char* s = genRandStr();
    Str str = init ( s );
    int a = rand() % (strlen(s)+100);
    char* spl = strndup(s,a);
    Str splited_str = *splitAt(str,a);
    TEST_ASSERT_EQUAL_STRING(spl, tocstr(splited_str) );
    TEST_ASSERT( eqs( init(spl), splited_str )  );
    free(s);
    free(str.syms);
  }
}


void test_strchr_and_strrchr() {
  for (int i = 0; i < STR_R_CHR_TEST_AMOUNT; ++i) {
    char* s = genRandStr();
    Str str = init ( s );
    int a = rand() % 0xff;
    char* p = strchr(s, a);
    Str ptr = Strchr(str, a);
    if (p == NULL) {
      TEST_ASSERT_EQUAL_INT( NULL, ptr.syms);
    }
    else {
      TEST_ASSERT_EQUAL_STRING( p, tocstr(ptr) );
      TEST_ASSERT( eqs( init(p), ptr ) );
    }
    p = strrchr(s, a);
    ptr = Strrchr(str, a);
    if (p == NULL) {
      TEST_ASSERT_EQUAL_INT( NULL, ptr.syms);
    }
    else {
      TEST_ASSERT_EQUAL_STRING( p, tocstr(ptr) );
      TEST_ASSERT( eqs( init(p), ptr ) );
    }
    free(s);
    free(str.syms);
  }
}

void test_strcmp() {
  for (int i = 0; i < STR_R_CHR_TEST_AMOUNT; ++i) {
    char* s1 = genRandStr();
    char* s2 = genRandStr();
    Str str1 = init ( s1 );
    Str str2 = init ( s2 );
    int r = strcmp(s1,s2);

    TEST_ASSERT_EQUAL_INT( r, Strcmp("%s %s", s1, s2      ) );
    TEST_ASSERT_EQUAL_INT( r, Strcmp("%s %S", s1, str2    ) );
    TEST_ASSERT_EQUAL_INT( r, Strcmp("%s %P", s1, &str2   ) );

    TEST_ASSERT_EQUAL_INT( r, Strcmp("%S %s", str1, s2    ) );
    TEST_ASSERT_EQUAL_INT( r, Strcmp("%S %S", str1, str2  ) );
    TEST_ASSERT_EQUAL_INT( r, Strcmp("%S %P", str1, &str2 ) );

    TEST_ASSERT_EQUAL_INT( r, Strcmp("%P %s", &str1, s2   ) );
    TEST_ASSERT_EQUAL_INT( r, Strcmp("%P %S", &str1, str2 ) );
    TEST_ASSERT_EQUAL_INT( r, Strcmp("%P %P", &str1, &str2) );

    free(s1);
    free(s2);
    free(str1.syms);
    free(str2.syms);

  }
}
