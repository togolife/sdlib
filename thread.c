#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "threadpool.h"

void printids (const char *s, int i) {
  pid_t pid;
  pthread_t tid;
  
  pid = getpid ();
  tid = pthread_self ();
  printf ("%s pid %u tid %u times %d\n", s, pid, tid, i);
}

void* thr_fn (void *arg) {
  int i = (int) arg;
  printids ("new thread: ", i);
}

int main (void) {
  TPool *pool = initPool (5);
  if (pool == NULL) {
    printf ("init pool failed!\n");
    return -1;
  }
  int i = 0;
  for (i = 0; i < 20; ++i) {
    Thread *t = getWork (pool);
    if (t == NULL) {
      printf ("get work failed! i == %d\n", i);
      continue;
    }
    set_work (t, thr_fn, (void *)i);
  }
  releasePool (pool);
  return 0;
}

