#include "main.h"



int Strcmp(const char* format,...) {
  va_list a;
  va_start(a,format);
  Str p[2];
  const char* sp=format;
  for (int i = 0; i < 2; ++i) {
    for (;; ++sp) {
      if (*sp == '%') break;
      if (*sp == '\0') {
/* production only:
        fprintf(stderr, "BAD FORMAT STRING: %s\n", format);      
*/
        exit(0);
      }
    }
    sp += 1;
    switch (*sp) {
      case 'P':
        p[i] = *va_arg(a, Str*);
        break;
      case 'S':
        p[i] = va_arg(a, Str);
        break;
      case 's':
        p[i].syms = va_arg(a, char*);
        p[i].len = -1;
        break;
      default:
/* production only:
        fprintf(stderr, "BAD FORMAT STRING: %s\n", format);      
*/
        exit(0);
    }
  }
  va_end(a);
  int i, j;
  unsigned char c1, c2;
  i = j = 0;
  if (p[0].len == -1 && p[1].len != -1) {
    do {
      c1 = (unsigned char) *p[0].syms++;
      c2 = (unsigned char) *p[1].syms++;
      j += 1;
      if ( c1 == '\0' || j == p[1].len)
        return c1 - c2;
    } while (c1 == c2);
  }
  else if (p[0].len == -1 && p[1].len == -1) {
    do {
      c1 = (unsigned char) *p[0].syms++;
      c2 = (unsigned char) *p[1].syms++;
      if ( c1 == '\0' )
        return c1 - c2;
    } while (c1 == c2);
  }
  else if (p[0].len != -1 && p[1].len == -1) {
    do {
      c1 = (unsigned char) *p[0].syms++;
      i += 1;
      c2 = (unsigned char) *p[1].syms++;
      if ( c1 == '\0' || i == p[0].len)
        return c1 - c2;
    } while (c1 == c2);
  }
  else if (p[0].len != -1 && p[1].len != -1) {
    do {
      c1 = (unsigned char) *p[0].syms++;
      i += 1;
      c2 = (unsigned char) *p[1].syms++;
      j += 1;
      if ( i == p[0].len || j == p[1].len )
        return c1 - c2;
    } while (c1 == c2);
  }
  return c1 - c2;
}

Str Strchr(Str a,char c) {
  Str res;
  res.len = a.len;
  char* p;
  for (p = a.syms; *p != c; ++p, --res.len) {
    if (*p == '\0') {
      res.syms = NULL;
      res.len = 0;
      return res;
    }
  }
  res.syms = (char*) malloc (length(res)*sizeof(char));
  memcpy(res.syms, p, res.len);
  return res;
}

Str Strrchr(Str a, char c) {
  Str res;
  res.len = a.len;
  char* last = NULL;
  for (char* p = a.syms; *p != '\0'; ++p) {
    if (*p == c) {
      if (last == NULL) res.len -= p - a.syms;
      else res.len -= p - last;
      last = p;
    }
  }
  if (last == NULL) {
    res.syms = NULL;
    res.len = 0;
    return res;
  }
  res.syms = (char*) malloc (length(res)*sizeof(char));
  memcpy(res.syms, last, res.len);
  return res;
}

Str* splitAt(Str a, int p) {
  Str* res = (Str*) calloc(2,sizeof(Str));
  if (p > length(a)) {
    res[0] = a;
    return res;
  }
  res[0].syms = (char*) malloc( (p) * sizeof(char));
  res[1].syms = (char*) malloc( ( length(a) - p ) * sizeof(char) );
  setlen(&res[0], p );
  setlen(&res[1], length(a) - p );
  memcpy(res[0].syms, a.syms, p );
  memcpy(res[1].syms, &a.syms[p], length(a) - p );
  return res;
}

void setlen(Str* a, int l) {
  a->len = l;
}

char* tocstr(Str a) {
  if (a.syms == NULL) return NULL;
  char* res = (char*) malloc(length(a)*sizeof(char));
  memcpy(res, a.syms, length(a));
  res[length(a)] = '\0';
  return res;
}

int length(Str a) {
  return a.len;
}

int printStr(Str a) {
  for (int i = 0; i < length(a); ++i) {
    putchar(a.syms[i]);
  }
}
Str scanStr() {
  Str res;
  scanf("%d",&res.len);
  res.syms = (char*) malloc(length(res)*sizeof(char));
  scanf("%s",res.syms);
  return res;
}

Str init (char* s) {
  Str res;
  res.len = strlen(s);
  res.syms = (char *) malloc( length(res) * sizeof(char)  );
  memcpy(res.syms, s, length(res));
  return res;
}

/* "%S %s", Str {3, "123"}, "456\0" = Str {n, "123 456"} */
Str form(const char* format,...) {
  va_list args;
  Str res;
  res.len = 0;
  int len=0;

  
  Str *tmp1, tmp2;
  char* tmp3;

  va_start(args,format);

  for (const char* c=format; *c != '\0'; ++c ) {
    if (*c == '%') {
      c+=1;
      switch (*c) {
        case 'P':
          tmp1 = va_arg(args,Str*);
          len += length(*tmp1);
          break;
        case 'S':
          tmp2 = va_arg(args,Str);
          len += length(tmp2);
          break; 
        case 's':
          tmp3 = va_arg(args,char*);
          len += strlen(tmp3);
          break;
        case '\0':
/* production only :
          fprintf(stderr, "FORMAT STRING ERROR in str:%s\n", format);
*/          
          //return NULL;
          exit(0);
        default:
/* production only :
          fprintf(stderr, "UNKNOWN FORMAT ERROR in str:%s\n", format);
*/          
          //return NULL;
          exit(0);
      }
    }
    else len+=1;
  } 
  va_end(args);
  va_start(args,format);
  int pos = 0;
  res.len = len;
  res.syms = (char*) malloc(len*sizeof(char));
  len = 0;
  for (const char* c = format; *c != '\0'; ++c) {
    if (*c == '%') {
      c += 1;
      switch (*c) {
        case 'P':
          tmp1 = va_arg(args, Str*);
          memcpy(&res.syms[pos], tmp1->syms, length(*tmp1)  ) ;
          pos += length(*tmp1);
          break;
        case 'S':
          tmp2 = va_arg(args,Str);
          memcpy(&res.syms[pos], tmp2.syms, length(tmp2)  ) ;
          pos += length(tmp2);
          break;
        case 's':
          tmp3 = va_arg(args,char*);
          len = strlen((char*)tmp3);
          memcpy(&res.syms[pos], tmp3, len  ) ;
          pos += len;
          break;
      }
    }
    else {
      res.syms[pos] = *c; 
      pos+=1;
    }
  }
  va_end(args);
  return res;
}

Str concatStr(Str a, Str b) {
  Str res;
  res.len = length(a) + length(b);
  res.syms = (char*) malloc(length(res)*sizeof(char));
  memcpy(res.syms, a.syms, length(a));
  memcpy(&res.syms[length(a)], b.syms, length(b));
  return res;
}



Str subs(Str a, int fr, int to) {
  Str res;
  if (fr > length(a) || fr > to) {
/* production only :
    fprintf(stderr, "BAD NUMBERS IN SUBS: %s, %d, %d\n", tocstr(a),fr,to);
*/
    setlen(&res,0);
    res.syms = NULL;
    return res;
  }
  if (to > length(a)) 
    to = length(a) - 1;
  setlen(&res, to-fr+1);
  res.syms = (char*) malloc(length(res)*sizeof(char));
  memcpy(res.syms, &a.syms[fr], length(res));
  return res;
}
