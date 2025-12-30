#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../util/utils.h"
#include <malloc.h>

#define ALLOCSTEP BUFSIZ 

#define ASZ 100

int readLines(FILE *f, char **lines) {
  int nl = 0;
  size_t n;
  while (getline(lines + nl, &n, f) > 0)
    nl++;
  return nl;
}

long columnToLong(char **lines, int nl, int i) {
  long res = 0;
  for(int j = 0; j < nl - 1; j++){
    char ch = lines[j][i];
    int digit = ch - '0';
    if ( 0 <= digit && digit <= 9) {
      res = 10 * res + digit;
    }
  }
  return res;
}

long tjoff(char op, int k, int j, char **lines, int nl) {
  long acc;
  switch (op) {
  case '+':
    acc = 0;
    break;
  case '*':
    acc = 1;
    break;
  default:
    break;
  }
  // printf("operator = %c\n", op);
  for (int i = k; i < j - 1; i++) {
    //printf("Examining column %d\n", i);
    long term = columnToLong(lines, nl, i);
    // printf("Found term %ld\n", term);
    switch(op) {
    case '+':
      acc += term;
      break;
    case '*':
      acc *= term;
      break;
    }

    // printf("acc = %ld\n", acc);
  }
  return acc;
}

void processFile(FILE *f) {
  int nl = 0; // number of lines
  char * lines[ASZ];
  
  for (int j = 0; j < ASZ; j++)
    lines[j] = NULL;
 
  nl = readLines(f, lines);

  int j = 0, k = -1;
  char ch, nextch;
  long long sum = 0;
  while(( nextch = lines[nl -1] [j])) {
    switch(nextch) {
    case '+':
    case '*':
      //printf("k = %d j = %d\n", k, j);
      if ( k >= 0) {
        long p = tjoff(ch, k, j, lines, nl);
        sum += p;
      }
      k = j;
      ch = nextch;
      break;
    default:
      break;
    }
    j++;
  }
  // Also do the last one
  //printf("LAST: k = %d j = %d\n", k, j); 
  sum += tjoff(ch, k, j, lines, nl);

  printf("Sum = %lld", sum);
}

int main(int argc, char **argv) {
  processFile(stdin);
}
