#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "sha256.h"
#include "utils.h"

/**
 * A structure that holds data associated with each thread.
 */
typedef struct ThreadData {
  int id;
  int dictIndexLow;
  int dictIndexHigh;
  pthread_t threadId;
} ThreadData;

/**
 * Structure to hold global data shared among threads
 */
typedef struct {
  const char *hashedPassword;
  int numThreads;
  int active;
  ThreadData *threadData;
} GlobalData;

//The single shared data instance
GlobalData sharedData;

/**
 * Function that brute-force searches through all dictionary words
 * specified by the given ThreadData (specified by an indexed range).
 * Each dictionary word is suffixed with all possible 3 digit numbers
 *
 * Returns normally if no match is found.  If a match is found this
 * function sets the globally shared flag value (GlobalData.active) to
 * false so that other threads can terminate.
 */
void *crackPassword(void *arg) {

  ThreadData *data = (ThreadData *) arg;
  printf("Thread %d hashing [%d, %d) = [%s, %s]...\n",
    data->id,
    data->dictIndexLow,
    data->dictIndexHigh,
    DICTIONARY[data->dictIndexLow],
    DICTIONARY[data->dictIndexHigh-1]);

    char password[255];
    char hash[67];
    //2 digit suffixes
    int m = 100;

    // if(data->dictIndexHigh != 247405) {
    //   return NULL;
    // }

    for(int i=data->dictIndexLow; i<data->dictIndexHigh; i++) {
      if(sharedData.active == 0) {
        //printf("Thread %d terminating (at %d)...\n", data->id, i);
        return NULL;
      }
      for(int j=0; j<m; j++) {
        //append 2 digits:
        sprintf(password, "%s%02d", DICTIONARY[i], j);
        sha256toHex(hash, password);

        if(strcmp(hash, sharedData.hashedPassword) == 0) {
          printf("Thread %d cracked password: %s => %s\n", data->id, sharedData.hashedPassword, password);
          printf("Signaling other threads to terminate...\n");
          sharedData.active = 0;
          return NULL;
        }

      }
    }

    printf("Thread %d did not crack password\n", data->id);
    return NULL;
}

int main(int argc, char **argv) {

  int numThreads = 4;

  if(argc < 2) {
    fprintf(stderr, "Usage: %s hashedPassword [numThreads=4]\n", argv[0]);
    exit(1);
  } else if(argc == 3) {
    numThreads = atoi(argv[2]);
  }

  initDictionary();

  ThreadData *threadData = (ThreadData *) malloc(sizeof(ThreadData) * numThreads);
  pthread_t *threadIds = (pthread_t *) malloc(sizeof(pthread_t) * numThreads);

  sharedData.hashedPassword = argv[1];
  sharedData.numThreads = numThreads;
  sharedData.active = 1;
  sharedData.threadData = threadData;

  printf("creating %d threads...", numThreads);
  for(int i=0; i<numThreads; i++) {
    threadData[i].id = (i+1);
    threadData[i].dictIndexLow = i * (NUM_WORDS/numThreads);
    threadData[i].dictIndexHigh = (i+1) * (NUM_WORDS/numThreads);
  }
  threadData[numThreads-1].dictIndexHigh = NUM_WORDS;
  printf("DONE\n");

  printf("Starting threads...\n");

  for(int i=0; i<numThreads; i++) {
    printf("Starting thread %d...\n", threadData[i].id);
    pthread_create(&threadIds[i], NULL, crackPassword, &threadData[i]);
    threadData[i].threadId = threadIds[i];
  }
  for(int i=0; i<numThreads; i++) {
    //block this main() thread until each one terminates:
    pthread_join(threadIds[i], NULL);
  }
  printf("All threads DONE\n");

  return 0;

}
