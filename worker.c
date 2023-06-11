#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>
#include "syncarray.h"

void *worker(void *arg)
{
  long counter = 0;
  syncarray *sa = (syncarray *) arg;
  int rc = 0;
  while ((rc != -1) && (counter < 25000))
  {
#if DEBUG
    printf("Storing %ld\n", counter);
#endif

    // Alternative use of synchronized array:
    // Demonstration of storing the system timer value in nanoseconds. 
    // This does not produce deterministic output.
    //struct timespec ts;
    //clock_gettime(CLOCK_REALTIME, &ts);
    //printf("val %ld\n",ts.tv_nsec);
    //rc = put(sa,ts.tv_nsec);

    // Store increasing counter in array
    // Should produce determinstic output - same value each time program is run
    counter ++;
    rc = put(sa,counter);
  }

  return NULL;
}

int main (int argc, char * argv[])
{
  pthread_t p1;
  pthread_t p2;
  syncarray sa;
  init(&sa);

  printf("Must run this program on a computer with at least 2 CPUs.\n");
  printf("The initial array sum is %ld\n",sum(&sa));
  
  // Launch threads
  pthread_create(&p1, NULL, worker, &sa);
  pthread_create(&p2, NULL, worker, &sa);

  pthread_join(p1, NULL);
  pthread_join(p2, NULL);

  printf("The final array sum is %ld\n",sum(&sa));
  printf("The avg array element is %ld\n",avg(&sa));
  
  // Print output
  return 0;
}
