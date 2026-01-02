#include <stdlib.h>
#include <stdio.h>

typedef struct box {
  int x[3];
} box;

#define ALLOCSTEP 12

struct box *readBoxes(FILE *f, int *n) {
  char *s = NULL;
  int l;
  size_t ns;
  int _n = 0;
  int space = 0;
  box *result = NULL;
  while((l =getline(&s, &ns,f)) > 0) {
    if (_n >= space) {
      space += ALLOCSTEP;
      result = realloc(result, space * sizeof(box));
    }
    //printf("Line: %s",s);
    int j = sscanf(s, "%d,%d,%d",&(result[_n].x[0]), &(result[_n].x[1]), &(result[_n].x[2]));
    // printf("Box: %d %d %d\n", result[_n].x[0], result[_n].x[1], result[_n].x[2]);
    _n++;
  }
  
  *n = _n;
  return result;
}
 
/* squared distance with dimension input*/
long dsq(int * x1, int *x2, int d) {
  long res = 0L;
  for( int i = 0; i < d; i++) {
    long di = (long) x1[i] - x2[i];
    res += di * di;
  }
  return res;
}

/* Box squared distance*/
long bdsq(box *b1, box *b2) {
  int *x1 = &(b1->x[0]);
  int *x2 = &(b2->x[0]);
  return  dsq(x1, x2, 3);
}  

void doDistances(box *b, int nb){
  for(int i = 0; i < nb; i++) {
    for (int j = i + 1; j < nb; j++) {
      printf(
        "box1: %d %d %d box2: %d %d %d d2: %ld\n",
        b[i].x[0],
        b[i].x[1],
        b[i].x[2],
        b[j].x[0],
        b[j].x[1],
        b[j].x[2],
        bdsq(b + i, b + j));
    }
  }
}

void processFile(FILE *f) {
  int nb;
  box *b = readBoxes(f, &nb);
  doDistances(b, nb);
}

int main(int argc, char **argv) {
  processFile(stdin);
}
