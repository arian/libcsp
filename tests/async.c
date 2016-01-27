#include <process.h>
#include <csp.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

Channel *enters = NULL;

void *push_enter() {
  int x = 1;
  usleep(1e6);
  while (1) {
    CSP_chanOutInt32(enters, x++);
    usleep(1e5);
  }
}

void print_value (Process *__csp_process__) {
  while (1) {
    signed int __csp_chan_read_temp_0;
    printf("%d\n", (CSP_chanInCopy(enters, &__csp_chan_read_temp_0, sizeof (signed int)), __csp_chan_read_temp_0));
    usleep(5e5);
  }
}


signed int main () {

  pthread_t enter_thread;
  pthread_create(&enter_thread, NULL, &push_enter, NULL);

  {
    if (enters == NULL) {
      Channel __enters_csp_channel;
      enters = &__enters_csp_channel;
      CSP_chanInit(enters, CSP_ONE2ONE_CHANNEL, 0);
      int asyncBuffer;
      CSP_chanSetAsync(enters, &asyncBuffer);
    }
    Process print_value_proc_2;
    ProcInit(&print_value_proc_2, print_value, NULL, 0, 0);
    ProcPar(&print_value_proc_2, NULL);
    ProcInitClean(&print_value_proc_2);
  }

  pthread_join(enter_thread, NULL);

  return 0;
}

