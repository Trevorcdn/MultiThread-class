#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "queue.h"
#include "queue.c"



void test_null_args(void){
  queue_t a;
  void** nothing = 0;
  a = queue_create();
  assert(queue_enqueue(NULL, NULL) == -1);
  assert(queue_enqueue(a, NULL) == -1);
  assert(queue_dequeue(NULL, NULL) == -1);
  assert(queue_dequeue(a,nothing) == -1); //dequeuing empty queue
  assert(queue_destroy(NULL) == -1);
  printf("NULL arugments all returns -1 (Working as intended)\n");
}

void test_dequeue_enqueue(void){
  queue_t p;
  /* Initialize the queue and enqueue items */
  p = queue_create();
  
  /*testing queue_length on NULL queue*/
  assert(queue_length(NULL) == -1);
  
  int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int *ptr = 0;
  for(int i = 0; i < 10; i++){
    queue_enqueue(p, &data[i]);
  }

  /*Checks if enqueue is working*/
  printf("Length of queue after 10 enqueue =  %d\n",queue_length(p));
  assert(queue_length(p) == 10);
  
  /*destorying queue when there are still elements*/
  assert(queue_destroy(p) == -1);
    
  /*Dequeue all elements*/
  for(int j = 0; j < 10; j++){
    queue_dequeue(p, (void *)&ptr);
  }

  printf("Size of queue after dequeueing all elements = %d\n",queue_length(p));
  assert(queue_length(p) == 0);
  /*queue can be destroyed now with size == 0*/
  assert(queue_destroy(p) == 0);
  printf("Queue destroyed successfully.\n");
}

//Given in Phase1 hints
/* Callback function that increments items by a certain value */
static int inc_item(void *data, void *arg)
{
  int *a = (int*)data;
  int inc = (int)(long)arg;

  *a += inc;

  return 0;
}

/* Callback function that finds a certain item according to its value */
static int find_item(void *data, void *arg)
{
  int *a = (int*)data;
  int match = (int)(long)arg;

  if(*a == match)
   return 1;

  return 0;
}

/*
 Code was given in phase1 hints and is used to test if queue_iterate
 is working correctly with find_item() and inc_item
*/
void test_iterator(void)
{
  queue_t q;
  int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int i;
  int *ptr = NULL;
  /* Initialize the queue and enqueue items */
  q = queue_create();
  for(i = 0; i < 10; i++)
    queue_enqueue(q, &data[i]);
    
  /* Add value '1' to every item of the queue */ 
  queue_iterate(q, inc_item, (void*)1, NULL);
  /* Test to see if the iterate function actually
   * added 1 to every value of the data array
   */
  assert(data[0] == 2);
  assert(data[1] == 3);
  assert(data[2] == 4);
  assert(data[3] == 5);
  assert(data[4] == 6);
  assert(data[5] == 7);
  assert(data[6] == 8);
  assert(data[7] == 9);
  assert(data[8] == 10);
  assert(data[9] == 11);

  /*Testing when queue or func is NULL*/
  assert(queue_iterate(NULL, find_item, (void*)5, (void **) &ptr) == -1);
  assert(queue_iterate(q, NULL, (void*)5, (void **) &ptr) == -1);
  
  /* Find and get the item which is equal to value '5' */
  queue_iterate(q, find_item, (void*)5, (void **) &ptr);
  assert(ptr != NULL);
  assert(ptr == &data[3]); //Piazza @179 address this
  assert(*ptr == 5);
}

void test_delete(void)
{
  queue_t j;
  int data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int i;

  /* Initialize the queue and enqueue items */
  j = queue_create();
  for(i = 0; i < 10; i++){
    queue_enqueue(j, &data[i]);
  }
  assert(queue_delete(j, NULL) == -1);
  /*Testing deletes from the head*/
  queue_delete(j, &data[0]);
  assert(*(int*)(j->head->data) == 1 && *(int*)(j->head->next->data) == 2);
    
  /*Testing deleting center nodes*/
  queue_delete(j, &data[2]);
  queue_delete(j, &data[3]);
  assert(*(int*)(j->head->next->next->data) == 5);
    
  /*Testing deleting nodes that aren't present*/
  assert(queue_delete(j, &data[0]) == -1);
  assert(queue_length(j) == 7);
    
  /*Testing whether moving the tail of the queue works*/
  queue_delete(j, &data[9]);
  assert(*(int*)(j->tail->data) == 8);
 
  /*Testing what happens if we delete all nodes (including the last node)
  removing from the tail forwards to also test the edge case of removing 
  the tail. */
  for(i = 9; i >= 0; i--){
    queue_delete(j,&data[i]);
  }
  assert(queue_length(j) == 0);
}


int main(){
   /* Testing NULL arguments being passed onto
      destroy,enqueue, and dequeue
    */
  printf("________________________________________________________\n");    
  printf("Testing NULL arguments for destroy,enqueue, and dequeue!\n");
  test_null_args();
    
  printf("________________________________________________________\n");
  printf("Testing enqueue, dequeue, and destory\n");
  test_dequeue_enqueue();
    
  printf("________________________________________________________\n");
  printf("Testing the queue_delete()\n");
  test_delete();
  printf("test_delete function passed all tests\n");
  
  printf("________________________________________________________\n");
  printf("Testing the queue_iterator()\n");
  test_iterator();
  printf("Everything is finished and correct!\n");
}