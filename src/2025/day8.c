#include <stdlib.h>
#include <stdio.h>
#include "../util/utils.h"

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

int longCmp(void *p, void *q) {
  long *pp = (long *)p;
  long *qq = (long *)q;
  long d = *pp - *qq;
  if (d < 0) return -1;
  else if ( d > 0) return 1;
  else return 0;
}

  
typedef struct {
  box *p1;
  box *p2;
  long dist;
} edge;

int edgeCmp(void *p, void *q) {
  edge *pp = (edge *) p;
  edge *qq = (edge *) q;
  long d = pp->dist - qq->dist;
  if ( d < 0) return -1;
  else if ( d > 0) return 1;
  else return 0;
}

void doDistances(box *b, int nb){
  heap h;
  int ncomb = nb * (nb - 1) / 2;
  h.storage = malloc(sizeof(void*) *ncomb);
  h.storageSize = ncomb;
  h.usedStorage = 0;
  h.cmp = edgeCmp;
  h.notify = NULL;
  edge * edges = malloc( sizeof(edge) * ncomb);
  int di = 0;
  for(int i = 0; i < nb; i++) {
    for (int j = i + 1; j < nb; j++) {
      long d = bdsq(b + i, b + j);
      edges[di].p1  = b + i;
      edges[di].p2  = b + j;
      edges[di].dist  = d;
      di++;
      /*printf(
        "box1: %d %d %d box2: %d %d %d d2: %ld\n",
        b[i].x[0],
        b[i].x[1],
        b[i].x[2],
        b[j].x[0],
        b[j].x[1],
        b[j].x[2],
        d);
      */
    }
  }
  for (int i = 0; i < ncomb; i++) {
   insert(&h, edges + i);
  }
  long * p = peekTop(&h);
  printf("Shortest: %ld\n", ((edge*)p)->dist ) ;
}

void processFile(FILE *f) {
  int nb;
  box *b = readBoxes(f, &nb);
  doDistances(b, nb);
}

int main(int argc, char **argv) {
  processFile(stdin);
}
