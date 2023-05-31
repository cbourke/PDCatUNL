#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

const int NUM_WORDS = 247406;

void initDictionary() {
  DICTIONARY = (char **) malloc(sizeof(char *) * NUM_WORDS);
  char *dictStr = (char *) malloc(sizeof(char) * (2502256 + 100));
  FILE *f = fopen("./dictionary.txt", "r");
  char buffer[100];
  int j = 0;
  for(int i=0; i<NUM_WORDS; i++) {
    fgets(buffer, 100, f);
    int n = strlen(buffer);
    strcpy(&dictStr[j], buffer);
    dictStr[j+n-1] = '\0';
    DICTIONARY[i] = &dictStr[j];
    j += n;
  }
  return;
}
