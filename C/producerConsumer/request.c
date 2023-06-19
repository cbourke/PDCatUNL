#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#ifndef REQUEST_H_INCLUDED
#define REQUEST_H_INCLUDED
#include "request.h"
#endif

#include "curl_utils.h"

double execute(Request req) {

  char url[1000];
  sprintf(url, "https://cse.unl.edu/~cbourke/PDCatUNL/delay.php?delay=%.2f", req.delay);
  char *rawXml = getContent(url);
  return req.delay;
}
