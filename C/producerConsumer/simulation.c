#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#include "queue.h"
#include "producer.h"
#include "consumer.h"

int main(int argc, char **argv) {

  srand(time(NULL));

  int numProducers = 3;
  int numConsumers = 3;

  if(argc == 3) {
    numProducers = atoi(argv[1]);
    numProducers = atoi(argv[2]);
  } else if(argc != 1) {
    fprintf(stderr, "Usage: %s numProducers numConsumers (both default to 3)\n", argv[0]);
    exit(1);
  }

  double intervalMin = 2.0;
  double intervalMax = 5.0;

  double delayMin = 2.0;
  double delayMax = 5.0;

  //allow up to 1000 requests
  initialize(1000);

  ProducerData *pData = (ProducerData*) malloc(sizeof(ProducerData) * numProducers);
  pthread_t *producers = (pthread_t *) malloc(sizeof(pthread_t) * numProducers);

  ConsumerData *cData = (ConsumerData*) malloc(sizeof(ConsumerData) * numConsumers);
  pthread_t *consumers = (pthread_t *) malloc(sizeof(pthread_t) * numConsumers);

  for(int i=0; i<numProducers; i++) {
    pData[i].id = (i+1);
    pData[i].intervalMin = intervalMin;
    pData[i].intervalMax = intervalMax;
    pData[i].delayMin = delayMin;
    pData[i].delayMax = delayMax;
    pthread_create(&producers[i], NULL, produce, &pData[i]);
  }

  for(int i=0; i<numConsumers; i++) {
    cData[i].id = (i+1);
    pthread_create(&consumers[i], NULL, consume, &cData[i]);
  }

  //run indefinitely:
  pthread_join(consumers[0], NULL);
  //alternatively simulate for 180 seconds = 3 minutes:
  //sleep(180);

  return 0;

}
