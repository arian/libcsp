#include <process.h>
#include <csp.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
typedef struct  {
  char *label;
  Channel *number;
} print_ProcessArgs;

void print (Process *__csp_process__) {
  print_ProcessArgs *args = __csp_process__->args;
  char *label = args->label;
  Channel *number = args->number;
  {
    while (1) {
      signed int count;
      CSP_chanInCopy(number, &count, sizeof (signed int));
      printf("%s: %d\n", label, count);
    }
  }
}
typedef struct  {
  Channel *c;
} random_thing_ProcessArgs;

void random_thing (Process *__csp_process__) {
  random_thing_ProcessArgs *args = __csp_process__->args;
  Channel *c = args->c;
  {
    signed int i = 0;
    while (1) {
      CSP_chanOutInt32(c, i++);
      usleep(rand() % 1000 * 100);
    }
  }
}

signed int main () {
  Channel *up;
  Channel __up_csp_channel;
  up = &__up_csp_channel;
  CSP_chanInit(up, CSP_ONE2ONE_CHANNEL, 0);
  {
    Process print_proc_1;
    print_ProcessArgs print_proc_1_args;
    print_proc_1_args.label = "A";
    print_proc_1_args.number = up;
    print_proc_1.args = &print_proc_1_args;
    ProcInit(&print_proc_1, print, NULL, 0, 0);
    Process print_proc_2;
    print_ProcessArgs print_proc_2_args;
    print_proc_2_args.label = "B";
    print_proc_2_args.number = up;
    print_proc_2.args = &print_proc_2_args;
    ProcInit(&print_proc_2, print, NULL, 0, 0);
    Process random_thing_proc_3;
    random_thing_ProcessArgs random_thing_proc_3_args;
    random_thing_proc_3_args.c = up;
    random_thing_proc_3.args = &random_thing_proc_3_args;
    ProcInit(&random_thing_proc_3, random_thing, NULL, 0, 0);
    ProcPar(&print_proc_1, &print_proc_2, &random_thing_proc_3, NULL);
    ProcInitClean(&print_proc_1);
    ProcInitClean(&print_proc_2);
    ProcInitClean(&random_thing_proc_3);
  }
  return 0;
}
