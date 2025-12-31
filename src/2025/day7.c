#include <stdlib.h>
#include <stdio.h>

int calculateSplits(FILE *f) {
  size_t n, nold, ntemp;
  char *s = NULL, *sold = NULL, *stemp;
  int nsplits = 0;
  int l;
  while((l = getline(&s, &n, f)) > 0) {
    // printf("Read line %s",s);
    if (sold) {
      // printf("Old line %s", sold);
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
      printf("Changed line: %s",s);
    }
    stemp = sold;
    sold = s;
    s = stemp;
    //printf("sold = %s", sold);
    ntemp = nold;
    nold = n;
    n = ntemp;
  }
  return nsplits;
} 
  
int main(int argc , char **argv) {
  int nsplit = calculateSplits(stdin);
  printf("There were %d splits\n", nsplit);
}
