#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


typedef struct range {
  uint64_t lower;
  uint64_t upper;
} range;

int getMaxJoltage(const char*s, size_t l) {
  int i,j;
  int max = 0;
  for (i = 0;  i < l - 1; i++) { // potential first battery
    int first = s[i] - '0';
    if( 10 * first + 9 <= max)
      continue;
    for (j = i + 1; j < l; j++) {
      int second = s[j] - '0';
      if (10 * first + second > max)
        max = 10 * first + second;
    }
  } 
  return max;
}

int stepSelection(int n, int k, int *s) {
  int l = k - 1;
  while (l >= 0) {
    if (s[l] < n - k + l){
      printf("Stepped at level %d\n", l);
      s[l]++;
      for (int m = l + 1; m < k; m++){
        s[m] = s[l] + (m - l);
      }
      return 1;
    }
    l--;
  }
  return 0;
}

void putSelection(int k, unsigned int*p) {
  printf("Selection: [");
  for (int i = 0; i < k; i++)
    printf(" %u,", p[i]);
  puts("]");
}

/* 200 over 2 = 100* 199, no problem, we'll see in pt 2*/
uint64_t getMaxJoltageN(int n, const char*s, size_t l) {
  unsigned int* p = malloc(n * sizeof (unsigned int));
  unsigned char* values = malloc(l * sizeof (unsigned char));
  for (int i = 0; i < l; i++)
    values[i] = (unsigned char) (s[i] - '0');
  for (int i = 0; i < n; i++)
    p[i] = (unsigned int) i;
  uint64_t max = 0;
  do {
    uint64_t val = 0;
    // putSelection(n, p);
    // printf("Examining digits: [");
    for (int j = 0; j < n; j++) {
      val = 10 * val + values[p[j]];
      // printf(" %u,", values[p[j]]);
    }
    //puts("]\n");
    if( val > max){
      max = val;
      // printf("New max: %lld\n", max);
    }
  } while(stepSelection(l,n,p));
  free(p);
  free(values);
  return max;
}

uint64_t getMJN2(int n, const char *s, size_t l) {
  // printf("Investigating %s", s);
  unsigned int *p = malloc(n * sizeof(unsigned int));
  unsigned char* vs = malloc ( l * sizeof (unsigned char));
  for (int i = 0; i < l; i++) {
    vs[i]  = s[i] - '0';
  }
  int j = 0; // start of pickable interval
  uint64_t val = 0;
  for (int i = 0; i < n; i++){
    // printf("About to pick the largest possible %d from %s", (n - i) , s + j);
    int k = j;
    int nextVal = -1;
    do {
      // printf("Inspecting position %d .\n", k);
      if(vs[k] > nextVal) {
        nextVal = vs[k];
        j = k + 1;
      }
      k++;
    } while( k < l - n + i + 1);
    // printf("Picked %u at position %d\n", (unsigned int) vs[j-1], j-1);
    val = 10 * val + nextVal;
    // printf("val = %lld\n", val);

  }
  free(p);
  free(vs);
  return val;
}

char * test[] = {
  "987654321111111",
  "811111111111119",
  "234234234234278",
  "818181911112111" 
};

void listSelections(int n, int k) { // list all selections of k elements out of n (use 0,1,2,3,4,...,n-1)
  int* p = malloc(k * sizeof(unsigned int));
  for (int i = 0; i < k; i++) {
    p[i] = i;
  }

  do {
    puts("Current selection:");
    for(int l = 0; l < k; l++)
      printf(" %d", p[l]);
    puts("\n");
  } while (stepSelection(n, k, p));
  free(p);
}

int main(int argc, char **argv) {
  //listSelections(7,3);
  uint64_t sum = 0;
  size_t n = 0;
  char* s = NULL;
  while(getline(&s, &n, stdin) > 0) {
   // printf("Read line \"%s\"\n", s);
   size_t lineLength = strlen(s);
   if(s[lineLength - 1] == '\n')
     lineLength--;
   // sum += getMaxJoltageN(12, s, lineLength);
   sum += getMJN2(12, s, lineLength);
   // printf("Sum = %lld\n", sum);
  } 

  printf("The maximum total joltage is %lld\n", sum);

}
