#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>
#include "syncarray.h"

// ---------------------------------
// Synchronized Array Data Structure
// ---------------------------------

#define ARRAY_SIZE 50000
#define DEBUG 2

pthread_mutex_t mutex; // Mutex for synchronization

struct ThreadData {
  syncarray* array; // Pointer to the synchronized array
  long startValue; // Starting value for populating the array
};

// init() Initialize Synchronized Array
void init(syncarray *c)
{
  int i;
  for (i = 0; i < ARRAY_SIZE; i++)
    c->value[i] = 0;
  c->index = 0;
}

// put() Put method: adds element with "val" value at current location
int put(syncarray *c, long val)
{
  pthread_mutex_lock(&mutex); // Acquire the lock before modifying the array

  int store_index = c->index;
  if (store_index < ARRAY_SIZE)
  {
    c->value[store_index] = val;
    c->index = c->index + 1;
  }
  else
    store_index = -1;

  pthread_mutex_unlock(&mutex); // Release the lock after modifying the array

  return store_index;
}

// get() Get method: returns element at specified position from synchronized array
long get(syncarray *c, int index)
{
  long rc = c->value[index];
  return rc;
}

// sum() Sum method: returns total of all elements in the array
long sum(syncarray *c)
{
  int i;
  long sum = 0;
  for (i = 0; i < ARRAY_SIZE; i++)
    sum += c->value[i];
  return sum;
}

// avg() Average method: returns the average element value for all elements in the array as a long
long avg(syncarray *c)
{
  return sum(c) / ARRAY_SIZE;
}

void* populateArray(void* arg)
{
  struct ThreadData* threadData = (struct ThreadData*)arg;
  syncarray* array = threadData->array;
  long startValue = threadData->startValue;
  long endValue = startValue + ARRAY_SIZE / DEBUG;

  for (long i = startValue; i < endValue; i++) {
    put(array, i);
  }

  return NULL;
}

