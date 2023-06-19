
/**
 * Consumer thread data.
 */
typedef struct {
  int id;
} ConsumerData;

/**
 * Consume function that continuously attempts to dequeue and
 * handle Requests from the queue.
 * 
 */
void *consume(void *arg);
