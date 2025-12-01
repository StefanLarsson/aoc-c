#include <stdlib.h>
#include <stdio.h>


#define LINE_BUFSIZ 10
#define ANGLE_STEPS 100

void do_day1(FILE* f) {
  char buf[LINE_BUFSIZ];
  char lrbuf[3];
  int state = 50;
  int nz = 0; // zeros at end of rotation
  int nz2 = 0; // zeros during rotations
  while (fgets(buf, LINE_BUFSIZ - 1, f)) {
    unsigned int degs;
    int n = sscanf(buf, "%[LR]%u",lrbuf,&degs);
    if( n == 2) {
      int nRounds = degs / ANGLE_STEPS;
      int rem = degs % ANGLE_STEPS;
      nz2 += nRounds;
      if (lrbuf[0] == 'L') {
        if (state != 0 && rem >= state)
          nz2++;
        state -= degs;
      }
      else if (lrbuf[0] == 'R') {
        if (rem >= ANGLE_STEPS - state)
          nz2++;
        state += degs;
      }
      state = state % ANGLE_STEPS;
      if (state < 0)
        state += ANGLE_STEPS;
      if ( state % ANGLE_STEPS == 0)
        nz++;
      // printf("buf = %s state = %d nz = %d nz2 = %d\n",buf, state, nz, nz2);
    }
  }
  printf("The dial was at zero %d times at ends of rotations\n", nz);
  printf("The dial was at zero %d times in total\n", nz2);

}
int main(int argc, char **argv) {
  do_day1(stdin);
}

