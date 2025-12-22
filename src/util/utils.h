#ifndef AOC_UTILS_H
#define AOC_UTILS_H
#include <stdio.h>
#include <stdlib.h>

typedef int (*heap_compare) (void *, void *);

typedef void (*heap_notify_idx)(void *, int);

typedef struct {
  void **storage;
  int storageSize;
  int usedStorage;
  heap_compare cmp;
  heap_notify_idx notify;
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

void insert(heap *h, void *n);

void* deleteTop(heap *h);
void *peekTop(heap *h);

void handlePossibleKeyDecrease(heap *h, int i);

int emptyHeap(heap *h);
#endif
