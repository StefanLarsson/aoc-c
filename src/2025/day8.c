#include <stdlib.h>
#include <stdio.h>
#include "../util/utils.h"

typedef struct box {
  int x[3];
  struct box *parent; // Disjoint set plan!
  int size;           // As above!
} box;

box* join(box* p1, box *p2) {
  while( p1->parent)
    p1 = p1->parent;
  while( p2->parent)
    p2 = p2->parent;
  if (p1 == p2) return p1;
  if(p1->size >= p2->size) {
      p1->size += p2->size;
      p2->parent = p1;
      return p1;
  } else {
      p2->size += p1->size;
      p1->parent = p2;
      return p2;
  }
}
#define ALLOCSTEP 12

struct box *readBoxes(FILE *f, int *n) {
  char *s = NULL;
  int l;
  size_t ns;
  int _n = 0;
  int space = 0;
  struct box *result = NULL;
  while((l =getline(&s, &ns,f)) > 0) {
    if (_n >= space) {
      space += ALLOCSTEP;
      result = realloc(result, space * sizeof(box));
    }
    //printf("Line: %s",s);
    int j = sscanf(s, "%d,%d,%d",&(result[_n].x[0]), &(result[_n].x[1]), &(result[_n].x[2]));
    result[_n].parent = NULL;
    result[_n].size = 1;
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

/*
 * Edges must be available and sufficient size
 */
void buildEdges(box *b, int nb, edge *edges){
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
}

void buildEdgeHeap(heap *h, edge *edges, int n) {
  h->storage = malloc(sizeof(void*) * n);
  h->storageSize = n;
  h->usedStorage = 0;
  h->cmp = edgeCmp;
  h->notify = NULL;
  for (int i = 0; i < n; i++) {
   insert(h, edges + i);
  }
}

int boxCmp(void *p, void *q) {
  box *pp =  p, *qq =  q;
  return qq->size - pp->size;
}
void buildBoxHeap(heap *h, box *boxes, int n) {
  h->storage = malloc(sizeof(void*) * n);
  h->storageSize = n;
  h->usedStorage = 0;
  h->cmp = boxCmp;
  h->notify = NULL;
  for (int i = 0; i < n; i++) {
   insert(h, boxes + i);
  }
}


void doDistances(box *b, int nb){
  int ncomb = nb * (nb - 1) / 2;
  edge * edges = malloc( sizeof(edge) * ncomb);
  buildEdges(b, nb, edges);

  heap h;
  buildEdgeHeap(&h, edges, ncomb);
  edge * p = peekTop(&h);
  // hack to handle example vs actual problem
  int nShortest = (nb < 100) ? 10 : 1000;
  for (int i = 0; i < nShortest; i++){
    edge *e = deleteTop(&h); 
    join(e->p1, e->p2);
  }
  heap hh;
  buildBoxHeap(&hh, b, nb);
  int prod = 1;
  for (int i = 0;i < 3; i++) {
    box * p = deleteTop(&hh);
    prod *= p->size;
  }
  printf("The product of the sizes of the three largest circuits is %d.\n", prod);
}

void processFile(FILE *f) {
  int nb;
  box *b = readBoxes(f, &nb);
  doDistances(b, nb);
}

int main(int argc, char **argv) {
  processFile(stdin);
}
