#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef REQUEST_H_INCLUDED
#define REQUEST_H_INCLUDED
#include "request.h"
#endif

#include "utils.h"
#include "consumer.h"
#include "queue.h"

void *consume(void *arg) {

  char timeBuffer[24];

  ConsumerData *data = (ConsumerData *) arg;

  sprint_time(timeBuffer);
  printf("%-23s Request handler (consumer) %d starting...\n", timeBuffer, data->id);

  Request req;

  while(true) {

    req = dequeue();
    sprint_time(timeBuffer);
    printf("%-23s Request handler (consumer) %d executing request #%d...\n", timeBuffer, data->id, req.id);
    double delaySeconds = execute(req);
    sprint_time(timeBuffer);
    printf("%-23s Request handler (consumer) %d finished request #%d (%.2f seconds)\n", timeBuffer, data->id, req.id, delaySeconds);

  }

}
