#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "utils.h"

void sprint_time(char buffer[]) {

  int millisec;
  struct tm* tm_info;
  struct timeval tv;

  gettimeofday(&tv, NULL);

  millisec = (int) (tv.tv_usec/1000.0);
  if (millisec>=1000) {
    millisec -=1000;
    tv.tv_sec++;
  }
  tm_info = localtime(&tv.tv_sec);

  strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
  sprintf(buffer, "%s.%03d", buffer, millisec);
}
