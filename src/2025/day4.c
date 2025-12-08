#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


uint64_t count_reachable(const char* s_prev, char *s, const char *s_next) {
  printf("Prev = %s", s_prev);
  printf("Current = %s", s);
  printf("Next = %s", s_next);
  puts("---------------");
  int l = strlen(s);
  if (s[l-1] == '\n'){
    s[l-1] = 0;
    l--;
  }

  int reachable = 0;
  for(int i = 0; i < l; i++) {
    if (s[i] == '@') {
      printf("A roll on position %d !\n", i);
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

int main(int argc, char **argv) {
  char *s = NULL, *s_prev = NULL, *s_next = NULL;
  size_t n = 0, n_prev = 0, n_next = 0;
  int l;
  int i = 0;
  FILE *f = stdin;
  char* s_temp;
  size_t n_temp;
  uint64_t sum = 0;
  while((l = getline(&s_next, &n_next, f)) > 0) {
    if(s) {
      sum += count_reachable(s_prev, s, s_next);
      
    }
    s_temp = s_prev;
    n_temp = n_prev;
    s_prev = s;
    n_prev = n;
    s = s_next;
    n = n_next;
    s_next = s_temp;
    n_next = n_temp;
  }

  // Also deal with the last one
  free(s_next);
  s_next = NULL;
  sum += count_reachable(s_prev, s, s_next);
  free(s_prev);
  free(s);
  printf("There are %lld reachable rolls of paper.\n", sum);
}
