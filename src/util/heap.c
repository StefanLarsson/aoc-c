#include <stdio.h>
#include <stdlib.h>
#include "./utils.h"

void handlePossibleKeyDecrease(heap *h, int i) {
  void **s = h->storage;
  while(i > 0 && h->cmp(s[P(i)] , s[i]) > 0) {
    void *t = s[P(i)];
    s[P(i)] = s[i];
    s[i] = t;
    if (h->notify)
      h->notify(s[i],i);
    i = P(i);
  }
  if (h->notify)
    h->notify(s[i], i);
}

void insert(heap *h, void *n) {
  void **s = h->storage;
  if(h->usedStorage >= h->storageSize) {
    printf("\n!!!!!!!!!!!!!!!! Error: overflow! \n");
    return;
  }

  s[h->usedStorage++] = n;
  int i = h->usedStorage - 1;

  handlePossibleKeyDecrease(h, i);
}



int emptyHeap(heap *h) {
  return h->usedStorage == 0;
}

void *peekTop(heap *h) {
  if (h->usedStorage)
    return h->storage[0];
  return NULL;
}

void* deleteTop(heap *h) {
  if (emptyHeap(h)) {
    printf("\n!!!!!!ERRORR!!!! deleting from empty heap");
    return 0;
  }
  void **s = h->storage;
  void *t = s[0];
  s[0] = s[--(h->usedStorage)];
  if (h->notify)
    h->notify(s[0],0);
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
    if (h->notify) {
      h->notify(s[i], i);
    }
    i = j;
    if (LC(i) < h->usedStorage && h->cmp(s[LC(i)] , s[i]) < 0) {
      j = LC(i);
    }
    if (RC(i) < h->usedStorage && h->cmp(s[RC(i)] , s[j]) < 0) {
      j = RC(i);
    }
  }
  if(h->notify && i < h->usedStorage)
    h->notify(s[i],i);

  return s[h->usedStorage];
}
