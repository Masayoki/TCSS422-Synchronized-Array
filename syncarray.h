#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>

// Global Address Space

#define ARRAY_SIZE 50000
#define NUM_THREADS 2

typedef struct __syncarray {
  long  value[ARRAY_SIZE];
  int  index;
}  syncarray;

void init(syncarray *c);
int put(syncarray *c, long val);
long get(syncarray *c, int index);
long sum(syncarray *c);
long avg(syncarray *c);

