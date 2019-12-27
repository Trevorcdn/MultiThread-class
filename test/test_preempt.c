/*
 * Testing preempt on threads
 *
 * Tests the creation of multiples threads and assure that uncooperative threads
 * could not keep the processing resouce for themselves if they never call
 * uthread_yield(). 
 *
 * For example, if preempt were not implemented, then the test program output 
 * below would be:
 *   In thread 1
 *   In thread 1
 *   In thread 1
 *   In thread 1
 * And so on. Basically, thread 1 would run indefinitely. However, with preempt
 * implemented, the output would switch between 'In thread 1' and 'In thread 2'
 * whenever the alarm is fired, which forces the currently running thread to
 * yield so another thread can be scheduled
 *
 * WARNING: THE PROGRAM MIGHT TAKES UP CPU
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <uthread.h>

int thread2(void* arg)
{
    while(1){
     printf("In thread %d\n", uthread_self());
    }
    return 0;
}

int thread1(void* arg)
{
    /*Add a timer so that the program wouldn't just run on forever*/
    time_t start = time(NULL);
    time_t seconds = start + 20; //End program after 20 seconds elapsed
    uthread_create(thread2, NULL);
    while(start < seconds){ 
	  printf("In thread %d\n", uthread_self());
	  start = time(NULL);
    }
    return 0;
	
}

int main(void)
{
    uthread_join(uthread_create(thread1, NULL), NULL);
    return 0;
}

