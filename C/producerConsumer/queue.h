
#ifndef REQUEST_H_INCLUDED
#define REQUEST_H_INCLUDED
#include "request.h"
#endif

/**
 * Initializes the blocking queue to have the given capacity.  This
 * function sets up the semaphores and mutex necessary to coordinate
 * its operations.
 */
void initialize(int queueCapacity);

/**
 * Shuts down the blocking queue and cleans up the resources
 * associated with it.
 */
void shutdown();

/**
 * Submit a Reqeust to the queue (in a thread-safe manner).
 * Blocks if the queue is full.
 */
void enqueue(Request req);

/**
 * Remove and retrieve the next available Reqeust in the queue (in a
 * thread-safe manner).  Blocks if no Request is available.
 */
Request dequeue();

/**
 * Returns the current number of elements in the queue.
 */
int getCurrentSize();

/**
 * A queue monitor that reports the size of the queue
 * every 5 seconds
 */
void *monitor(void *arg);
