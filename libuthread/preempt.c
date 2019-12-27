#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "preempt.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

void preempt_disable(void)
{
  /* TODO Phase 4 */
  sigset_t block_alarm;
  sigprocmask(SIG_BLOCK, &block_alarm, NULL);
}

void preempt_enable(void)
{
  /* TODO Phase 4 */
  sigset_t unblock_alarm;
  sigprocmask(SIG_UNBLOCK, &unblock_alarm, NULL);
}

void termination_handler(int signum){
 /* Everytime the alarm is fired, we force the currently running 
  * thread to yield so another thread can be scheduled */
  uthread_yield();
}

void preempt_start(void)
{
	/* TODO Phase 4 */
  sigset_t start_alarm;
  struct sigaction sa;
  struct itimerval alarm;
  
  /*Installing signal handler that recieves
    alarm signals of type SIGVTALRM */
  sa.sa_handler = termination_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGVTALRM, &sa, NULL);
  
  /* Setting up intervals:
   * usec is measured in mircoseconds & there are 1,000,000 usec per second.
   * The frequency of preemption is 100Hz(100 times per second) so the time 
   * measured is [(1sec)/HZ]*[(1,000,000usec)/(1sec)]
   */
  alarm.it_interval.tv_usec = 1000000/HZ;
  alarm.it_interval.tv_sec = 0;
  alarm.it_value.tv_usec = 1000000/HZ;
  alarm.it_value.tv_sec = 0;
  setitimer (ITIMER_VIRTUAL, &alarm, NULL);
  
  sigemptyset(&start_alarm);
  sigaddset(&start_alarm, SIGVTALRM);
}

