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

typedef struct {
  uint64_t low;
  uint64_t high;
} range;

void processIdRange(char* s, range* p) {
  sscanf(s, "%lld-%lld", &(p->low), &(p->high));
  //printf("low = %lld high = %lld\n", p->low, p->high);
}

int inRange(uint64_t id, range* p) {
  return p->low <= id && id <= p->high;
}

int inAnyRange(uint64_t id, range* p, int n) {
  int i = 0;
  while( i < n) {
    if( inRange(id, p+i))
      return 1;
    i++;
  }
  return 0;
}

int readRanges(FILE *f, range *p) {
  int l;
  size_t n;
  char *s = NULL;
  int nR = 0;
  while((l = getline(&s, &n, f)) > 0 && strlen(s) > 1) {
    //printf("Read %s", s);
    processIdRange(s, p + (nR++));
  }
  free(s);
  return nR;
}

int readAndCheckIds(FILE* f, range* r, int nR) {
  int nFresh = 0;
  int l;
  size_t n;
  char *s = NULL;
  while( (l = getline(&s, &n, f)) > 0 ){
    uint64_t id;

    // printf("Read %s", s);
    sscanf(s, "%lld", &id);
    if( inAnyRange(id, r, nR))
      nFresh++;
  } 
  return nFresh;
}

int compareRanges(void *pp, void *qq) {

  range *p = (range*) pp;
  range *q = (range*) qq;

  if (p->low < q->low)
    return -1;
  if (p->low > q-> low)
    return +1;
  if (p->high < q->high)
    return -1;
  if (p->high > q->high)
    return +1;
  return 0;
}

void buildRangeHeap(heap *h, range *r, int nR) {
  h->storage = malloc(nR * sizeof(range *));
  h->usedStorage = 0;
  h->cmp = compareRanges;
  h->notify = NULL;
  while(h->usedStorage < nR) {
    insert(h,r + h->usedStorage);
  }
}

uint64_t processRange(range *p)
{
  printf("Combined range: %lld - %lld\n", p->low, p->high);
  return 1 + p->high - p->low;
}

void processInput(FILE *f) {
  int l;
  size_t n;
  char *s = NULL;
  heap h;

  range* p = malloc(sizeof(range) * 300);

  int nR = readRanges(f, p);


  buildRangeHeap(&h, p, nR);

  uint64_t totalFresh = 0;
  if(!emptyHeap(&h)) {
    range *working = (range*) deleteTop(&h);
    while(!emptyHeap(&h)) {
      range* candidate = (range*) deleteTop(&h);

      if(candidate->low <= working->high + 1) {
        if (candidate->high > working->high)
          working->high = candidate->high;
        printf("  Combining range: %lld - %lld\n", working->low, working->high);
      } else {
        printf("  ");
        totalFresh += processRange(working);
        printf("  totalFresh = %llu\n", totalFresh); 
        working = candidate;
      }
    }
    totalFresh += processRange(working);
    printf("totalFresh = %llu\n", totalFresh); 
  }

  while (!emptyHeap(&h)) {
    range* q = (range*) deleteTop(&h);
    printf("%lld - %lld\n", q->low, q->high);
  }
  int m = 0;
  int nFresh = readAndCheckIds(f, p, nR);
  printf("Fresh %d IDs\n", nFresh);
  printf("Total fresh IDs %llu\n", totalFresh);
  free(s);
}

      

  
int main(int argc, char **argv) {
  processInput(stdin);
}
