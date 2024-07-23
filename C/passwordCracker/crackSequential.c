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

  char *hashedPassword = argv[1];
  char hash[67];
  char password[255];
  int i, j;

  printf("Loading dictionary with %d words...\n", NUM_WORDS);
  initDictionary();

  //search all 2 digit appended numbers...
  int m = 100;

  for(i=0; i<NUM_WORDS; i++) {
    for(j=0; j<m; j++) {
      //create password padded with 2 digits
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
