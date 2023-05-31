#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>

#include "sha256.h"

int main(int argc, char **argv) {

  int numThreads = 4;

  if(argc < 2 || argc > 3) {
    fprintf(stderr, "Usage: %s password [numThreads]\n", argv[0]);
    exit(1);
  } else if(argc == 3) {
    numThreads = atoi(argv[2]);
  }
  char *hashedPassword = argv[1];
  char hash[67];
  char password[255];
  int counter = 0, i, j;

  omp_set_num_threads(numThreads);

  //search all 2 digit appended numbers...
  int m = 100;

  //private: copy for each thread
  //shared: each thread should only read
  //schedule: guided = chunk size starts off large and decreases to better handle load imbalance between iterations.
  //          2000 = minimum chunk size to use
  #pragma omp parallel for private(password,j),shared(hashedPassword,m,words),schedule(guided, 2000)
  for(i=0; i<NUM_WORDS; i++) {
    for(j=0; j<m; j++) {
      //create password
      sprintf(password, "%s%02d", DICTIONARY[i], j);
      sha256toHex(hash, password);

      if(strcmp(hash, hashedPassword) == 0) {
        printf("Cracked password: %s => %s\n", hashedPassword, password);
        exit(0);
      }

    }
  }

  printf("Password not found!\n");

  return 0;

}
