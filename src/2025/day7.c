#include <stdlib.h>
#include <stdio.h>

int calculateSplits(FILE *f) {
  size_t n, nold, ntemp;
  char *s = NULL, *sold = NULL, *stemp;
  int nsplits = 0;
  int l, lold;
  long long *counts = NULL, *countsold = NULL, *countstemp;
  while((l = getline(&s, &n, f)) > 0) {
    counts = realloc(counts, l * sizeof(long));
    for (int i = 0; i < l; counts[i++] = 0);

    
    // printf("Line: %s",s);
    if (sold) {
      // printf("Old line %s", sold);
      for ( int i = 0; i < l; i++) {
        switch(s[i]) {
        case '.':
          counts[i] += countsold[i];
          break;
        case '^':
          counts[i-1] += countsold[i];
          counts[i+1] += countsold[i];
          break;
        default:
          break;
        }
        
      }  
      for ( int i = 0; i < l; i++) {
        switch(s[i]) {
        case '.':
          if (sold[i] == '|' || sold[i] == 'S')
            s[i] = '|';
          break;
        case '^':
          if (sold[i] == '|' ){
            s[i-1] = s[i+1] = '|';
            nsplits++;
          }
          break;
        default:
          break;
        }
        
      }  
      //printf("Changed line: %s",s);
    } else {
      for ( int i = 0; i < l; i++) {
        counts[i] = (s[i] == 'S') ? 1 : 0;
      } 
    }
/*
    printf("      ");
    for (int i =0; i < l; i++){
      if(counts[i])
        printf("%lld", counts[i]);
      else
        printf("%c", s[i]);
    }
    printf("      ");
    for (int i = 0; countsold && i < l; i++) {
      printf("%lld", countsold[i]);
    } 
    puts("");
*/
    stemp = sold;
    sold = s;
    s = stemp;
    //printf("sold = %s", sold);
    ntemp = nold;
    nold = n;
    n = ntemp;
    countstemp = countsold;
    countsold = counts;
    counts = countstemp;
    lold = l;
  }
  long long sum = 0;
  for (int i = 0; i < lold; i++) {
   // printf("%lld\n", countsold[i]);
   sum += countsold[i];
  }
  printf("Paths %lld\n", sum);
  return nsplits;
} 
  
int main(int argc , char **argv) {
  int nsplit = calculateSplits(stdin);
  printf("There were %d splits\n", nsplit);
}
