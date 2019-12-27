# Thought Process/explanation

## queue API ##
```
struct Node 
{
  void *data;
  struct Node *next;
};

struct queue {
	struct Node *head;
	struct Node *tail;
	int queueSize;
};
```
*node* struct is used to represent an element within the queue and contains
a pointer to the potential next node. Meanwhile, the *queue* struct is used to
hold a pointer of struct Nodes that contain the queue's head and tail. It also
has an int that records the size of the queue.

test_queue.c test the function queue_iterate. The test cases that made sure the 
queue is deleting, enqueuing, dequeuing, destorying, creating, and 
queue_length are as intended. Added multiple test cases involving edge cases.

## uthread API ##
```
typedef struct tcb{
  uthread_t TID;
  ucontext_t context; //back up of CPU registers
  void *stack;
  int state;
  int isJoined;
}tcb_t;

```
A typedef struct called tcb that represents the execution context of each thread. 
state is an int that represent the state of a thread, where the number 0 
means it is ready, 1 means it is running, 2 means it is blocked, and 3 means 
the state is a zombie. The int 'isJoined' checks if a currentThread has been 
currently joined yet since "a thread can be joined by only one other thread". 

While implementing *uthread_yield*, the struct tcb has variables 
that contains the prevThread and the nextThreads. Then I enqueue the 
currentThread so that it would be at the end of the queue, and then I would
dequeue so that the next thread could run. However, I noticed that within this
method, the line *prevThread = currentThread* only works for programs with one
thread. To fix that, an if statement to check the queue's length is equal
to 1 is added. If it did, then set prevThread = currentThread. Otherwise,
set *prevThread = nextThreads*. There's also a while loop that checks if the
next thread is ready, and if it is not then enqueue and dequeue until a thread 
is found that is ready to execute.

*uthread_join* uses the queue_iterate function to
access the thread with the TID that is given by the user. When it is found, 
mark the thread so that no other threads can join it again, transfer it to
a second queue tracking blocked objects, and then switch the context to the next
thread. When exiting, checked if the thread exiting is one that has been joined on.
If so, search our blockedQueue for the thread it is joined to, update it's status


## Sources ##
1. https://www.gnu.org/software/libc/manual/html_mono/libc.html#Testing-for-Delivery
2. https://www.gnu.org/software/libc/manual/html_mono/libc.html#System-V-contexts
3. https://www.gnu.org/software/libc/manual/html_mono/libc.html#Sigaction-Function-Example
 
```
