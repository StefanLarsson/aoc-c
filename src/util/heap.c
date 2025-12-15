#include <stdio.h>
#include <stdlib.h>

typedef int (*heap_compare) (void *, void *);

typedef struct {
  void **storage;
  int storageSize;
  int usedStorage;
  heap_compare cmp;
} heap;

/*
        0
   1          2
 3   4     5     6
7 8 9 10 11 12 13 14
left child of i = 2 * i + 1
right child of i = 2 * i + 2
parent of i = (i - 1) / 2  (integer division)

*/
#define LC(i) (1 + 2 * (i))
#define RC(i) (2 + 2 * (i))
#define P(i) ((i) - 1) / 2

void insert(heap *h, void *n) {
  void **s = h->storage;
  if(h->usedStorage >= h->storageSize) {
    printf("\n!!!!!!!!!!!!!!!! Error: overflow! \n");
    return;
  }

  s[h->usedStorage++] = n;
  int i = h->usedStorage - 1;

  while(i > 0 && h->cmp(s[P(i)] , s[i]) > 0) {
    void *t = s[P(i)];
    s[P(i)] = s[i];
    s[i] = t;
    i = P(i);
  }
}

int emptyHeap(heap *h) {
  return h->usedStorage == 0;
}

void* deleteTop(heap *h) {
  if (emptyHeap(h)) {
    printf("\n!!!!!!ERRORR!!!! deleting from empty heap");
    return 0;
  }
  void **s = h->storage;
  void *t = s[0];
  s[0] = s[--(h->usedStorage)];
  s[h->usedStorage] = t;
  int i = 0;
  int j = i;
  if (LC(i) < h->usedStorage && h->cmp(s[LC(i)] , s[i]) < 0) {
    j = LC(i);
  }
  if (RC(i) < h->usedStorage && h->cmp(s[RC(i)], s[j]) < 0) {
    j = RC(i);
  }
  while ( j > i) {
    t = s[i];
    s[i] = s[j];
    s[j] = t;
    i = j;
    if (LC(i) < h->usedStorage && h->cmp(s[LC(i)] , s[i]) < 0) {
      j = LC(i);
    }
    if (RC(i) < h->usedStorage && h->cmp(s[RC(i)] , s[j]) < 0) {
      j = RC(i);
    }
  }

  return s[h->usedStorage];
}


void printHeap(heap *h) {
  int rowLength = 1;
  int i = 0;
  printf("--\n");
  while ( i < h->usedStorage) {
    int rowEnd = i + rowLength;
    while ( i < rowEnd && i < h->usedStorage) {
      printf("%d ", *((int*) (h->storage[i])));
      i++;
    }
    rowLength *= 2;
    printf("\n");
  }
  printf("--\n");
}

int vs[] = { 34,65,1,3,43,23,12,21, 345,345,5345,454,54,0,0,32,232};

int intCmp(void *p, void *q) {
  return *((int*)p) - *((int *) q);
}

int main(int argc, char **argv) {
  printf("Hello, world!\n");
  heap h;
  h.storage = malloc(sizeof(void*) * BUFSIZ);
  h.storageSize = BUFSIZ;
  h.usedStorage = 0;
  h.cmp = intCmp;
  for (int i = 0; i < sizeof(vs) / sizeof(vs[0]); i++) {
    insert(&h, &(vs[i])); 
    printHeap(&h);

  }
  while(!emptyHeap(&h)) {
    printf("%d\n", *((int*) (deleteTop(&h))));
  }
  free(h.storage);

}


