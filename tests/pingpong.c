#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../src/process.h"

Channel *up;
Channel *down;

void random_up(Process *p) {
  while (1) {
    CSP_chanOutInt8(up, 1);
    usleep((rand() % 1000) * 1000);
  }
}

void random_down(Process *p) {
  while (1) {
    CSP_chanOutInt8(down, 1);
    usleep((rand() % 1000) * 1000);
  }
}

int is_ok_1(void * counter) {
  int a = *(int *) counter;
  return a < 5;
}

int is_ok_2(void *counter) {
  int a = *(int *) counter;
  return a > -5;
}

void counter(Process *p) {
  signed int count;
  signed int v;
  while (1) {
    {
      CSP_Alt_t alt;
      Channel *clist[2];
      struct CSP_Alt_Guard g1 = {&is_ok_1, &count};
      struct CSP_Alt_Guard g2 = {&is_ok_2, &count};
      clist[0] = up;
      clist[0]->altGuard = &g1;
      clist[1] = down;
      clist[1]->altGuard = &g2;
      CSP_altInit(&alt);

      switch (CSP_priAltSelect(&alt, clist, 2)) {
        case 0:
            {
              signed int tmp;
              CSP_chanInCopy(up, &tmp, sizeof (signed int));
            }
            count++;
        break ;
        case 1:
          {
            CSP_chanInCopy(down, &v, sizeof (signed int));
            count -= v;
          }
        break ;
      }
      CSP_altClose(&alt);
    }
    printf("counter: %d\n", count);
  }
}

int main() {
  Channel up_;
  up = &up_;
  CSP_chanInit(up, CSP_ONE2ONE_CHANNEL, 0);

  Channel down_;
  down = &down_;
  CSP_chanInit(down, CSP_ONE2ONE_CHANNEL, 0);

  Process p_up;
  ProcInit(&p_up, random_up, NULL, 0, 0);

  Process p_down;
  ProcInit(&p_down, random_down, NULL, 0, 0);

  Process p_count;
  ProcInit(&p_count, counter, NULL, 0, 0);

  ProcPar(&p_up, &p_down, &p_count, NULL);

  return 0;
}
