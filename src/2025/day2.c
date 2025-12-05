#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


typedef struct range {
  uint64_t lower;
  uint64_t upper;
} range;

int getNumber(FILE *f, uint64_t *result) {
  int c;
  int read = 0;
  *result = 0;
  do {
    c = fgetc(f);
    if ( '0' <= c && c <= '9') {
      *result = 10 * (*result) + c - '0';
      // printf("result = %ld\n", *result);
      read = 1;
    } else {
      ungetc(c, f);
      break ; 
    }
  } while(c != EOF);
  return read;
}

int getSpecificChar(FILE* f, unsigned char c) {
  int cc = fgetc(f);
  if (cc == c)
    return 1;
  ungetc(cc, f);
  return 0;
}

int getRange(FILE *f, range *r) {
  int n;
  n = getNumber(f, &(r->lower));
  if ( n == 0)
    return 0;
  n = getSpecificChar(f, '-');
  if ( n == 0)
    return 0;
  n = getNumber(f, &(r->upper));
  if ( n == 0)
    return 0;
  return 1;
}

static unsigned char tjoff[100];

int isRepeated2(unsigned char* s) {
  int l = strlen(s);
  if ( l % 2)
    return 0;
  for(int i = 0; 2 * i < l; i++) {
    if(s[i] != s[i+ (l / 2)])
      return 0;
  }
  return 1;
}

int isRepeatedAny(unsigned char* s) {
  int repFound = 0;
  int l = strlen(s);
  if ( l <= 1)
    return 0;
  int nReps = 2;
  while (nReps <= l) {
    if(l % nReps == 0) {
      int runLength = l/ nReps ;
      int i;
      int broken = 0;
      for (i = 0;i < runLength; i++) {
        int j;
        for(j = 0; j < nReps; j++) {
          if (s[i] != s[j * runLength + i]){
            broken = 1;
            break;
          }
        }
        if(broken)
          break;
      }
      if(!broken) {
        // printf("[%s] has %d repetitions of run length %d\n", s, nReps, runLength);

        return 1;
      }
    }
    nReps++;
  }

  return 0;
}

int isRepeated(uint64_t x) {
  sprintf(tjoff, "%lld", x);
  return isRepeatedAny(tjoff);
}

/* What is a repeated number? 
 * If 1 digit repeating: xxxxx = x * 1 + x * 10 + ...  = x * 11111
 * If 2 digit repeating:
 * xyxyxy = xy + xy * 100 + xy * 10000 = xy * (1 + 100 + 10000) = xy * 10101
 * If 3 digits repeating:
 * xyzxyzxyz = xyz * 1 + xyz * 1000 + xyx * 1000000 = xyz * (1001001)
 */

uint64_t processRange(range *r) {
  uint64_t sumRepeated = 0;
//  printf("Range: lower = %ld upper = %ld\n", r->lower, r->upper);
  for(uint64_t i = r-> lower; i <= r->upper; i++) {
    if(isRepeated(i)) {
//      printf("Invalid: %lld\n", i);

      sumRepeated+=i;
    } 
  }
//  printf("Sum of repeitions in range is  %lld repetitions in range\n", sumRepeated);
  return sumRepeated;
}

void loopRanges(FILE* f) {
  range r;
  int n;
  uint64_t sum = 0;
  while(n = getRange(f, &r)) {
    sum += processRange(&r);
    n = getSpecificChar(f, ',');
    if (!n)
      break;
  }

  printf("The sum of all the invalid serial numbers is %lld\n", sum);

}
 
  

void sumOfInvalidSerials() {
  uint64_t myint;
  int nRead;
  range r;
  
  nRead = getRange(stdin, &r);
  printf("Read %d ranges, result is lower: %ld upper: %ld\n", nRead, r.lower, r.upper);
}

  
int main(int argc, char **argv) {
  loopRanges(stdin);
}

