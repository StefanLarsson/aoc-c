#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../util/utils.h"


uint64_t count_reachable(const char* s_prev, char *s, const char *s_next) {
  puts("---------------");
  int l = strlen(s);
  if (s[l-1] == '\n'){
    s[l-1] = 0;
    l--;
  }

  int reachable = 0;
  for(int i = 0; i < l; i++) {
    if (s[i] == '@') {
      // printf("A roll on position %d !\n", i);
      int neighbours = 0;
      if (s_prev) {
        if (i > 0 && s_prev[i-1] == '@')
          neighbours++;
        if (s_prev[i] == '@')
          neighbours++;
        if (i < l - 1 && s_prev[i+1] == '@')
          neighbours++;
      }
      if(i > 0 && s[i-1] == '@')
        neighbours++;
      if(i < l - 1 && s[i+1] == '@')
        neighbours++;
      if(s_next) {
        if (i > 0 && s_next[i-1] == '@')
          neighbours++;
        if (s_next[i] == '@')
          neighbours++;
        if (i < l - 1 && s_next[i+1] == '@')
          neighbours++;
      }
      if (neighbours < 4)
        reachable++;
    }
  }
  return reachable;
}


int findNext(char c, char*s, int* curr) {
  while(s[*curr] != c && s[*curr] != 0){
    (*curr)++;
  }
  if(s[*curr] == 0){
   return 0;
  }
  return 1;
}  

typedef struct countedRoll {
  int x;
  int y;
  struct countedRoll* neighbours[8];
  unsigned int neighbourCount;
  int heapIdx;
} countedRoll;

void noteNeighbours(countedRoll* p, countedRoll *q) {
  p->neighbours[p->neighbourCount++] = q;
  q->neighbours[q->neighbourCount++] = p;
}



void countEarlierNeighbours(countedRoll *p, int i) {
  int j = i-1;
  while(j >= 0 && p[j].y >= p[i].y - 1) {
    if (p[j].x >= p[i].x - 1  && p[j].x <= p[i].x + 1) {
      //printf("Detected neighbours: (%d,%d) and (%d,%d)\n", p[i].x, p[i].y, p[j].x, p[j].y);
      noteNeighbours(&p[i], &p[j]);
    }
    j--;
  }
}



void displayRolls(countedRoll *p, int n) {
  int i;
  for (i = 0; i < n; i++) {
    printf("(%d,%d), neighborCount = %d\n", p[i].x, p[i].y, p[i].neighbourCount);
  }
}
    

int cmp(void *p, void *q) {
  return ((countedRoll*) p)->neighbourCount - ((countedRoll *) q)->neighbourCount ;
}

void notify(void* p, int idx) {
  countedRoll *r = (countedRoll*) p;
  r->heapIdx = idx;
}


int fjuff(FILE *f, countedRoll *p) {
  int l;
  char *s = NULL;
  size_t n = 0;
  int k = 0; // number or found rolls
  int i = 0;

  while((l = getline(&s, &n, f)) > 0) {
    int j = 0;
    //printf("Line %d: %s",i, s);
    while(findNext('@', s, &j)){
      p[k].x = j;
      p[k].y = i;
      p[k].neighbourCount = 0 ;
      p[k].heapIdx = -1;
      countEarlierNeighbours(p, k);
      //printf("%d\n ", j);
      j++;
      k++;
    }
    i++;
  }
  free(s);
  return k;
}


int removeNeighbour(countedRoll * p, countedRoll* q) {
  // remove p from neighbours of q, if it is there
  if (p == q) {
    printf("ERRORROROROROROR\n");
  }
  //printf("START: index is %d\n", q->heapIdx);
  int i = 0;
  while(i < q->neighbourCount && q->neighbours[i] != p)
    i++;
  if (i < q->neighbourCount) {
    // printf("Removing roll at (%d,%d) from the %d neighbours of roll at (%d,%d) at index %d\n", p->x, p->y,q->neighbourCount, q[i].x, q[i].y, q[i].heapIdx);
    q->neighbourCount--;
    while ( i < q->neighbourCount) {
      // printf("MIDDLE: index is %d\n", q->heapIdx);
      q->neighbours[i] = q->neighbours[i+1];
      i++;
    }
    // printf("Done removing roll\n");
    // printf("END: index is %d\n", q->heapIdx);
    return 1;
  }
  return 0;
}


int verifyHeap(heap* h) {
  for (int i = 0; i < h->usedStorage; i++) {
    if (LC(i) < h->usedStorage && h->cmp(h->storage[i], h->storage[LC(i)]) > 0) {
      printf("ERROR! LEFT CHILD (%d) OF INDEX %d IS SMALLER!\n", LC(i),  i);
      return 0;
    }
    if (RC(i) < h-> usedStorage && h->cmp(h->storage[i], h->storage[RC(i)]) > 0) {
      printf("ERROR! RIGHT CHILD (%d) OF INDEX %d IS SMALLER!\n",RC(i),  i);
      return 0;
    }
    if (((countedRoll*) (h->storage)[i])->heapIdx != i) {
      printf("ERROR! Heap index not maintained in application object!\n");
    }
  }
  return 1;
}

void displayHeap(heap *h) {
  printf("HEAP:\n");
  for (int i = 0; i < h->usedStorage; i++) {
    countedRoll *p = (countedRoll*) h->storage[i]; 
    printf("  (%d,%d) %d\n", p->x, p->y, p->neighbourCount);
  }
  printf("ENDHEAP\n");
}

int main(int argc, char **argv) {
  countedRoll* p = malloc(sizeof(countedRoll) * 20000);

  int k = fjuff(stdin, p);
  printf("Read %d rolls\n", k);
  // Gathered them up, sorted on y then x
  //
  //displayRolls(p, k);
  //

  heap h;
  h.cmp = cmp;
  h.usedStorage = 0;
  h.storageSize = k;
  h.storage = malloc(sizeof(countedRoll *) * k);
  h.notify = notify;

  int m = 0;
  for (m = 0; m < k; m++) {
    insert(&h, &(p[m]));
  }

  m = 0;
  countedRoll* r;
  while((r = (countedRoll*) peekTop(&h)) != NULL && r->neighbourCount <= 3) {
    if (r->neighbourCount > 3) {
      printf("  WHJICH IS > 3\n");
    }

    verifyHeap(&h);
    countedRoll *r = (countedRoll*) (deleteTop(&h));
    int o;
    for(o = 0; o < r->neighbourCount; o++) {
      if(removeNeighbour(r, r->neighbours[o])) {
        handlePossibleKeyDecrease(&h, r->neighbours[o]->heapIdx);
      }
    }
    m++;
  }

  printf("Removed %d rolls from heap\n", m);
  free(p);
  free(h.storage);
}
