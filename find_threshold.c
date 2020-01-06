#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "tsc.h"

#define NUM_MAXES 10
#define NUM_LOOPS 1000
#define STRING_LENGTH 1024 * 1024 * 1024
// #define DEBUG 1

void update_maxes(u_int64_t *maxes, u_int64_t time, int startIdx) {
#ifdef DEBUG
  if (startIdx == 0) {
    printf("%lu\n", time);
  }
#endif
  if (time > maxes[startIdx]) {
    u_int64_t tmp = maxes[startIdx];
    maxes[startIdx] = time;
    time = tmp;
  }
  if (startIdx + 1 < NUM_MAXES) {
    update_maxes(maxes, time, startIdx + 1);
  }
}

int main(int argc, char **argv) {

#ifdef DEBUG
  printf("Test start: \n");
#endif
  u_int64_t *maxThreshold = malloc(NUM_MAXES * sizeof(u_int64_t));
  start_counter();

  // just so that we've called this function once before timing it.
  get_counter();

  // Find the max time it takes to read the counter twice
#ifdef DEBUG
  printf("Find timer overhead: \n");
#endif
  for (int i = 0; i < NUM_LOOPS; i++) {
    u_int64_t clockStart = get_counter();
    u_int64_t clockStop = get_counter();
    update_maxes(maxThreshold, clockStop - clockStart, 0);
  }

  // Find the max time it takes to have a cache / tlb miss
  char *long_string = malloc(STRING_LENGTH * sizeof(char));
  memset(long_string, 0, STRING_LENGTH * sizeof(char));
  char tmp = 0;

#ifdef DEBUG
  printf("Find cache miss time: \n");
#endif

  for(int i = 0; i < NUM_LOOPS; i++) {
    // For each trial, access a single char, and time how long it takes
    u_int64_t idx = i * (STRING_LENGTH / NUM_LOOPS);
    
    u_int64_t clockStart = get_counter();
    tmp = long_string[idx];
    u_int64_t clockStop = get_counter();
    
    update_maxes(maxThreshold, clockStop - clockStart, 0);
  }

  // Return the median of the 10 maxes

#ifdef DEBUG
  printf("Printing maxes: \n");
  for (int i = 0; i < NUM_MAXES; i++) {
    printf("%lu\n", maxThreshold[i]);
  }

  printf("Median (multiplied by 3 as a fuzz factor): \n");
#endif
  printf("%lu\n", maxThreshold[5] * 3);
 
  return 0;
}
