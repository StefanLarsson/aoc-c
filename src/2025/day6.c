#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../util/utils.h"
#include <malloc.h>

#define ALLOCSTEP 8 

int parseAsInts(char*s, long** p, size_t* size, size_t *used) {
  int nc; // number of chars used
  int l; // Number of parsed ints in scan
  int n = 0; // Number of parsed int in total
  do {
    if (*used + 5 > *size) {
      *size += ALLOCSTEP;
      *p = realloc(*p, sizeof(long) * (*size));
    }
    l = sscanf(s,
      " %ld %ld %ld %ld %ld%n",
      *p + *used,
      *p + *used + 1,
      *p + *used + 2,
      *p + *used + 3,
      *p + *used + 4,
      &nc);
    if ( l == EOF){
      break;
    }
    s += nc;
    n += l;
    *used += l;
  } while ( l == 5);
  return n;
}


int parseOperatorLine(char *s) {
  char* nextFree = s;
  char* next = s;
  while (*next) {
    if(*next == '*' || *next == '+') {
      *nextFree++ = *next;
    }
    next++;
  }
  *nextFree = 0;
  return 1;
}
      
    
long problem(char *ops, long *x, size_t xs, int i) {
  int n = strlen(ops);
  char op = ops[i];
  long acc;
  if (op == '+'){
    acc = 0;
  } else if (op == '*') {
    acc = 1;
  }
  while (i < xs) {
    if (op == '*') {
      acc *= x[i];
    } else {
      acc += x[i];
    }
    i += n;
  }
  return acc;
}
  
  
void processFile(FILE *f) {
  char *s = NULL;
  size_t n;
  long *x = NULL;
  size_t currentSize = 0;
  size_t currentUsed = 0;
  int r = 0;
  int ll; // line length
  int nl = 0; // number of lines
  int ni = 0;
  
  while((ll = getline(&s, &n,f )) > 0 && (ni = parseAsInts(s, &x, &currentSize, &currentUsed)));

  // printf("ni = %d\n", ni);
  if (ni  != 0) {
    printf("Missing operator line!\n");
    exit(1);
  }
  // printf("Last string: %s",s);
  if (!parseOperatorLine(s)){
    printf("Failed parsing operators!}n");
    exit(EXIT_FAILURE);
  }
  // printf("Last string with operators compressed: %s\n", s);
  int no = strlen(s); // number of operators
  long sum = 0;
  for ( int i = 0; i < no; i++) {
    long p = problem(s, x, currentUsed, i);
//    printf("Problem %d has answer %ld\n", i, p );
    sum += p;
  } 
  printf("Sum of problems = %ld\n", sum);
}

int main(int argc, char **argv) {
  processFile(stdin);
}
