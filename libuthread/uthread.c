#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "queue.c"
#include "uthread.h"

/* TODO Phase 2 */

/* private data strucure tcb needs to hold the TID,
 * the context of the thread, information about its
 * stack & info about the state of the thread
 */
typedef struct tcb{
  uthread_t TID;
  ucontext_t context; //back up of CPU registers
  void *stack;
  int state;
  int isJoined;
  /* For state:
   * 0: State is ready
   * 1: State is running
   * 2: state is blocked
   * 3: state is zombie
   */
}tcb_t;

/*Holds pointer to current thread and next thread*/ 
struct tcb *currentThread;
struct tcb *nextThreads;
/*Queue API to keep track of tcb*/
queue_t tcbQueue;
queue_t blockedQueue;


static int access_thread(void *data, void *arg){
  struct tcb *test = (tcb_t*)(data);
  int tid = (int)(long)arg;
  if(tid == test->TID){
    return 1;
    }
  else return 0;
}

void uthread_yield(void){
  /* TODO Phase 2 */
  /* prevThread = currentThread works for programs with only
   * one yield call. Otherwise, prevThread->TID would just 
   * stay as zero if length of queue was > 1. As a result,
   * having prevThread = nextThread swaps the address of 
   * prevThread allowing it to have the correct TID
   */
  preempt_disable();  
  struct tcb *prevThread = (struct tcb*)malloc(sizeof(struct tcb));
  prevThread = currentThread;
  prevThread->state = 0; //READY state
  queue_dequeue(tcbQueue, (void **)&nextThreads);
  queue_enqueue(tcbQueue, (void *)prevThread);
  nextThreads->state = 1; //RUNNING state
  currentThread = nextThreads;
  preempt_enable(); 
  uthread_ctx_switch(&(prevThread->context), &(nextThreads->context));
}

uthread_t uthread_self(void){
  /* TODO Phase 2 */
  return nextThreads->TID;
  /*return TID of current thread;*/
}


int uthread_first_call(int user_threadID){
  /* When uthread_create() is called for the first time,
   * this function is used to first initialize the
   * uthread library. Returns -1 if fail and 0 if succeed
   */
  struct tcb *thread = (struct tcb*)malloc(sizeof(struct tcb));
  /*If thread fails to malloc,return -1*/
  if(thread == NULL)
    return -1;
    
  thread->TID = user_threadID;
  thread->stack = uthread_ctx_alloc_stack();
  tcbQueue = queue_create();
  blockedQueue = queue_create();
  thread->state = 0;
  thread->isJoined = -1;
  currentThread = thread;
  nextThreads = thread;
  preempt_start();
  return 0;
}

int uthread_create(uthread_func_t func, void *arg){
  /* TODO Phase 2 */
  static unsigned int user_threadID = 0;
  /*initializing the main thread*/
  if(user_threadID == 0){
    int succeed = uthread_first_call(user_threadID);
    if(succeed == 0){
      user_threadID++;
    }
    else{
      return -1;
    }		
  }
  /* If the main thread already exists, then creates the first
   * user thread & registers it to library using uthread_ctx_init
   */
  preempt_disable(); 
  struct tcb *thread = (struct tcb*)malloc(sizeof(struct tcb));
  /*If thread fails to malloc,return -1*/
  if(thread == NULL)
   return -1;
  thread->TID = user_threadID;
  thread->stack = uthread_ctx_alloc_stack();
  thread->state = 0;
  thread->isJoined = -1;
  
  preempt_enable(); 
  uthread_ctx_init(&(thread->context), thread->stack , func, arg);
  if(queue_enqueue(tcbQueue, (void*)thread) != 0){
   //enqueue failed and returns -1     
   return -1;
  }
  else{
   user_threadID++;
   return thread->TID;
  }
   
}

void uthread_exit(int retval)
{
  /* TODO Phase 2 */
  /* Since in uthread_yield we swap and made prevThread = nextThread,
   * we have to continue with the swap here in order for things to
   * work correctly
   */
  preempt_disable();  
  if(currentThread->isJoined != -1){
    long int castTID = currentThread->isJoined;
    struct tcb *joinedThread = NULL;
    queue_iterate(blockedQueue, access_thread, (void*)castTID, (void**)&joinedThread);
    joinedThread->state = 0;
    queue_enqueue(tcbQueue, (void *)joinedThread);
    }
  struct tcb *prevThread = currentThread;
  prevThread->state = 3; //ZOMBIE state
  queue_enqueue(tcbQueue, (void *)prevThread);
  queue_dequeue(tcbQueue, (void **)&nextThreads);
  nextThreads->state = 1; //RUNNING state
  currentThread = nextThreads;  
  preempt_enable(); 
  uthread_ctx_switch(&(prevThread->context), &(nextThreads->context));
 }

int uthread_join(uthread_t tid, int *retval){	
  preempt_disable(); 
  long int castTID = tid;
  struct tcb *joinedThread = NULL;
  struct tcb *prevThread = currentThread;
  queue_iterate(tcbQueue, access_thread, (void*)castTID, (void**)&joinedThread);
  joinedThread->isJoined = currentThread->TID;
  if(joinedThread->state != 3){
    currentThread->state = 2;
    queue_enqueue(blockedQueue, (void*)prevThread);
    queue_dequeue(tcbQueue, (void**)&currentThread);
    preempt_enable(); 
    uthread_ctx_switch(&(prevThread->context), &(currentThread->context));
    }
  else{
  //queue_delete(tcbQueue, 
  }
}
