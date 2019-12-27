#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "queue.h"

struct Node {
   void *data;
   struct Node *next;
};

struct queue{
   /* TODO Phase 1 */
   struct Node *head;
   struct Node *tail;
   int queueSize;
};

queue_t queue_create(void)
{	
   /* TODO Phase 1 */
   queue_t queue = (queue_t)malloc(sizeof(queue_t)); 
   if(queue == NULL)
     return NULL;
	
   queue->head = NULL;
   queue->tail = NULL;
   queue->queueSize = 0;
   return queue;
}

int queue_destroy(queue_t queue)
{
   /* TODO Phase 1 */
   /*Only can destroy when queue is empty*/
   if(queue == NULL || queue->queueSize > 0)
     return -1;
   else{ 
     free(queue);
     return 0;
   }
}

int queue_enqueue(queue_t queue, void *data)
{
    /* TODO Phase 1 */
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
 
    if(queue == NULL || data == NULL || newNode == NULL)
      return -1;
    
    if(queue->queueSize == 0){
      newNode->data = data;
      newNode->next = NULL;
      queue->head = newNode;
      queue->tail = newNode;
      queue->queueSize++;
      return 0;
    }
    else{
      newNode->data = data;
      newNode->next = NULL;
      queue->tail->next = newNode;
      queue->tail = newNode;
      queue->queueSize++;
      return 0;
    }
    
}

int queue_dequeue(queue_t queue, void **data)
{
    /* TODO Phase 1 */
    if(queue == NULL || data == NULL || queue->queueSize == 0)
      return -1;
    
    *data = queue->head->data;
    
    if(queue->queueSize == 1){
      free(queue->head);
      queue->queueSize--;
      return 0;
     }
    else{
      free(queue->head);
      queue->head = queue->head->next;
      queue->queueSize--;
      return 0;
    }
}

int queue_delete(queue_t queue, void *data)
{
    /* TODO Phase 1 */
    //Return: -1 if @queue or @data are NULL, of if @data was not found in the queue
    if(queue == NULL || data == NULL)
     return -1;
   
    struct Node **nodeTracker = &(queue->head); 
    struct Node **nodeToRemove;
    struct Node **nodeToReLink;
    int found = 0;
    if(queue->queueSize == 1 && (int*)(queue->head->data) == (int*)data) {
      free(queue->head);  
      queue->queueSize--;
      return 0;
    }
   
     /*Check Head Node to see if the data matches due to special case*/
    if((int*)((*nodeTracker)->data) == (int*)data){
      free(queue->head);
      queue->head = (*nodeTracker)->next;
      queue->queueSize--;
      return 0;
    }
    else{
        while((*nodeTracker)->next != NULL){
           if((int*)((*nodeTracker)->next->data) == (int*)data){ 
             nodeToRemove = &(*nodeTracker)->next;
             nodeToReLink = &(*nodeToRemove)->next;
             if(*nodeToRemove == queue->tail){
               queue->tail = *nodeTracker;
             }
             free(*nodeToRemove);
             (*nodeTracker)->next = *nodeToReLink;
             found = 1;
             queue->queueSize--;
             break;
          }
           nodeTracker = &((*nodeTracker)->next);
        }
    }
   
  if(found == 1){return 0;}
  return -1;
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	/* TODO Phase 1 */
    if(queue == NULL || func == NULL)
      return -1;
    else{  
    struct Node *currNode = queue->head;
    while(currNode != NULL){
        /*func returns 1 for a specific item and apply the func onto the data using the arg*/
      if(func(currNode->data,arg) == 1){
          /*
            If func complete it's function then store the modified data
            onto the original data (Similar to queue_dequeue in terms of void**)
          */
           *data = currNode->data;
       }
      
      currNode = currNode->next;
     }
     return 0;
    }
}

int queue_length(queue_t queue)
{
   /* TODO Phase 1 */
   if(queue == NULL)
      return -1;
   else
      return queue->queueSize;
}
