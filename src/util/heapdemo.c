#include <stdio.h>
#include <stdlib.h>
#include "./utils.h"


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


  *((int*) h.storage[7]) -= 30;
  handlePossibleKeyDecrease(&h, 7);

  printHeap(&h);

  while(!emptyHeap(&h)) {
    printf("%d\n", *((int*) (deleteTop(&h))));
  }




  free(h.storage);

}


