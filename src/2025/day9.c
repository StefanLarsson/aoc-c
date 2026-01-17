#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int x;
  int y;
} redtile;

#define ALLOCSTEP 5
  

long rectangleArea(redtile* p, redtile *q) {
  //printf("Calculating area of rectangle");
  //printf("with corners at (%d, %d) and (%d, %d)\n", p->x, p->y, q->x, q->y);
  long dx = (long) p->x - q->x;
  long dy = (long) p->y - q->y;
  if (dx < 0) dx = - dx;
  if (dy < 0) dy = - dy;
  long res = (dx + 1) * ( dy +1);
  //printf("The area is %ld\n", res);
  
  return res;
}

// since no clear definition of "inside", guessing all segments
// are axis-aligned
int isAllowed(redtile *p, int n, int i, int j) {
  int minx = p[i].x;
  int maxx = p[j].x;
  if (minx > maxx) {
    minx = maxx;
    maxx = p[i].x;
  }
  int miny = p[i].y;
  int maxy = p[j].y;
  if (maxy < miny) {
    miny = maxy;
    maxy = p[i].y;
  }
/**
 *
 * We have a rectangle
 *
 *    minx                 maxx
 *    |                      |
 *    +----------------------+- maxy
 *    |                      |
 *    |                      |
 *    +----------------------+- miny                 
 *
 *    and a line segment
 *
 *
 *       (p[k].x, p[k].y) - (p[l].x, p[l].y)
 *
 *   It is forbidden if the line segment crosses the rectangle border
 *   We know (assume) the line segment is axis-aligned!
 *   
 */
  int l;
  int result = true;
  for(int k = 0; result && k <= n; k++){
    l = (k + 1) % n;
    int x1 = p[k].x;
    int x2 = p[l].x;
    int y1 = p[k].y;
    int y2 = p[l].y;
    if ((x1 - minx) * (x2 - minx) <= 0 && miny < y1 && y1 < maxy)
      result = false;
    if ((x1 - maxx) * (x2 - maxx) <= 0 && miny < y1 && y1 < maxy)
      result = false;
    if ((y1 - miny) * (y2 - miny) <= 0 && minx < x1 && x1 < maxx)
      result = false;
    if ((y1 - maxy) * (y2 - maxy) <= 0 && minx < x1 && x1 < maxx)
      result = false;
   if (!result) {
    printf("Stopping rectangle (%d, %d) - (%d, %d)", minx, miny, maxx, maxy);
    printf(" due to line segment (%d, %d) - (%d, %d)\n", 
      x1,
      y1,
      x2,
      y2);
    }
  }
  return result;
}

long largestArea(redtile *p, int n) {
  long max = 0;
  long maxRedGreen = 0;
  for (int i = 0; i < n; i++) {
    for(int j = i + 1; j < n; j++) {
      long a = rectangleArea(p + i, p + j);
      if ( a > max)
        max = a;
      if (a > maxRedGreen && isAllowed(p, n, i, j)) {
        maxRedGreen = a;
        printf("Largest found redgreen: (%d, %d) - (%d, %d): %d\n", p[i].x, p[i].y, p[j].x, p[j].y, a);
      }
    }
  }
  printf("maxredgreen = %ld\n", maxRedGreen);
  return max;
}


int processFile(FILE *f) {
  char *s;
  size_t n;
  int l;
  int nn = 0, size = 0;
  redtile *p = NULL;
  
  while((l = getline(&s, &n, f)) > 0) {
    if(nn >= size) {
      size += ALLOCSTEP;
      p = realloc(p, size * sizeof(redtile));
    }
    //printf("Line: %s",s);
    redtile t;
    l = sscanf(s, "%d,%d", &(p[nn].x), &(p[nn].y));
    //printf("Red tile at %d %d\n", p[nn].x, p[nn].y);
    nn++;
  }
  for(int i = 0; i < nn; i++) {
    //printf("Red tile at %d %d\n", p[i].x, p[i].y);
  }
  printf(
    "The largest possible area of a rectangle with corners on red tiles is %ld.\n",
    largestArea(p, nn));
}

int main(int argc, char **argv) {
  return processFile(stdin);
}

