#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sha256.h"
#include "utils.h"

int main(int argc, char **argv) {

  if(argc != 2) {
    fprintf(stderr, "Usage: %s password\n", argv[0]);
    exit(1);
  }

  initDictionary();

  char *hashedPassword = argv[1];
  char hash[67];
  char password[255];
  int counter = 0;

  int m = 100;
  for(int i=0; i<NUM_WORDS; i++) {
    for(int j=0; j<m; j++) {
      //create password padded with 2 digits
      sprintf(password, "%s%02d", DICTIONARY[i], j);
      sha256toHex(hash, password);
      counter++;

      if(strcmp(hash, hashedPassword) == 0) {
        printf("Cracked password: %s => %s\n", hashedPassword, password);
        printf("Hashes Computed: %d\n", counter);
        exit(0);
      }

    }
  }

  printf("Password not found!\n");
  printf("Hashes Computed: %d\n", counter);

  return 0;

}
