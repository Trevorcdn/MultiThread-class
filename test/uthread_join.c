/*
 * Thread creation and joining test
 *
 * Tests the creation of multiples threads and the fact that a parent thread
 * should get returned to before its child is executed. Also, test if a thread
 * is able to successfully call the join function on another thread
 *
 * thread1
 * thread2
 * thread3
 */

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

int thread3(void* arg)
{
	printf("thread%d\n", uthread_self());
	return 0;
}

int thread2(void* arg)
{
	uthread_t tid3 = uthread_create(thread3, NULL);
	uthread_join(tid3, NULL);
	printf("thread%d\n", uthread_self());
	
	return 0;
}

int thread1(void* arg)
{
	uthread_t tid2 = uthread_create(thread2, NULL);
	uthread_join(tid2, NULL);
	printf("thread%d\n", uthread_self());
	
	return 0;
}

int main(void)
{
    uthread_t tid1 = uthread_create(thread1, NULL);
	uthread_join(tid1, NULL);
	return 0;
}
