
/**
 * Producer thread data
 */
typedef struct {
  int id;

  double intervalMin;
  double intervalMax;

  double delayMin;
  double delayMax;
} ProducerData;

/**
 * Produce function that continuously produces Requests and (attempts)
 * to enqueue them into the queue.
 * 
 */
void *produce(void *arg);
