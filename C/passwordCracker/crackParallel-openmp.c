#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
//TODO: add code here

#include "sha256.h"
#include "utils.h"

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
  int i, j;

  printf("Loading dictionary with %d words...\n", NUM_WORDS);
  initDictionary();

  printf("Using %d threads...\n", numThreads);
  //TODO: add code here

  //search all 2 digit appended numbers...
  int m = 100;

  //TODO: add code here
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
