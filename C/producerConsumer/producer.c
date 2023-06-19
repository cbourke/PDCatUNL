#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#ifndef REQUEST_H_INCLUDED
#define REQUEST_H_INCLUDED
#include "request.h"
#endif

#include "utils.h"
#include "queue.h"
#include "producer.h"

void *produce(void *arg) {

  char timeBuffer[24];

  ProducerData *data = (ProducerData *) arg;

  sprint_time(timeBuffer);
  printf("%-23s Request generator (producer) %d starting...\n", timeBuffer, data->id);

  //struct timespec nsTime = {0, 0};
  Request req = {0, 2.0};

  while(true) {

    //1. (initialize) a new request
    req.id = rand() % 10000;
    req.delay = data->delayMin + (data->delayMax - data->delayMin) * ( (double) rand() / RAND_MAX );
    sprint_time(timeBuffer);
    printf("%-23s Request generator (producer) %d submitting new request # %d\n", timeBuffer, data->id, req.id);

    enqueue(req);

    //sleep for a random interval between data->intervalMin and
    //                                    data->intervalMax
    double seconds = data->intervalMin + (data->intervalMax - data->intervalMin) * ( (double) rand() / RAND_MAX );
    struct timespec tim;
    tim.tv_sec = seconds;
    tim.tv_nsec = (long) (1000000000L * (seconds - tim.tv_sec));
    //printf("DEBUG: Request generator (producer) %d sleeping for %f seconds...\n", data->id, seconds);
    nanosleep(&tim, NULL);

  }
}
