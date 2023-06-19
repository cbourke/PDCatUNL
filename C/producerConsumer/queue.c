#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#ifndef REQUEST_H_INCLUDED
#define REQUEST_H_INCLUDED
#include "request.h"
#endif

#include "utils.h"
#include "queue.h"

/**
 * The semaphores needed to coordinate the blocking queue.
 *
 * A semaphore is a variable used to control access to a shared resource,
 * in this case the (blocking) queue.  Each semaphore is a *counter*
 * that you can increment (using sem_post()) or decrement (using sem_wait()).
 * However, if the value is zero, the thread is blocked until it becomes
 * positive.
 */
static sem_t capacityRemaining;
static sem_t currentSize;

/**
 * A mutex (mutual exclusion) lock that places a lock on the queue as a resource
 * so that when one producer or consumer is adding/removing an element, all
 * others are blocked.  Once the producer/consumer has finished adding/removing
 * a request, it releases the lock.
 */
static pthread_mutex_t queue_mutex_lock;

/**
 * This is the actual blocking queue that holds Request structures.
 */
static Request *queue;

/**
 * The "head" of the queue which refers to the next element to be
 * dequeued (removed) from the queue.
 */
static int headIndex;

/**
 * The "tail" of the queue which refers to the end of the queue and
 * the next available slot to enqueue a Request.
 */
static int tailIndex;

/**
 * The capacity of the queue.  When the queue is full, producers are
 * generally blocked from submitting new requests until room becomes
 * available.
 */
static int capacity;

void initialize(int queueCapacity) {
  capacity = queueCapacity;
  headIndex = 0;
  tailIndex = 0;
  queue = (Request *) malloc( sizeof(Request) * capacity );

  pthread_mutex_init(&queue_mutex_lock, NULL);
  sem_init(&capacityRemaining, 0, capacity);
  sem_init(&currentSize, 0, 0);

  //initiate the queue monitor
  pthread_t queueMonitor;
  pthread_create(&queueMonitor, NULL, monitor, NULL);

  return;
}

void shutdown() {

  pthread_mutex_destroy(&queue_mutex_lock);
  sem_destroy(&capacityRemaining);
  sem_destroy(&currentSize);
  free(queue);

  return;
}

void enqueue(Request req) {

  //if remaining capacity is zero, we wait...
  sem_wait(&capacityRemaining);

  //we lock the queue to enter the critical section
  pthread_mutex_lock(&queue_mutex_lock);

  queue[tailIndex] = req;
  tailIndex = (tailIndex + 1) % capacity;

  //unlock
  pthread_mutex_unlock(&queue_mutex_lock);

  sem_post(&currentSize);

}

Request dequeue() {

  Request result;

  //if currentSize is zero, we wait...
  sem_wait(&currentSize);
  //we lock the queue to enter the critical section
  pthread_mutex_lock(&queue_mutex_lock);

  result = queue[headIndex];
  headIndex = (headIndex + 1) % capacity;

  //unlock
  pthread_mutex_unlock(&queue_mutex_lock);

  sem_post(&capacityRemaining);

  return result;
}

int getCurrentSize() {
  int val;
  sem_getvalue(&currentSize, &val);
  return val;
}

void *monitor(void *arg) {

  char timeBuffer[24];

  while(true) {
    sleep(5);
    sprint_time(timeBuffer);
    printf("%-23s Queue monitor: %d requests awaiting execution.\n", timeBuffer, getCurrentSize());
  }
}
