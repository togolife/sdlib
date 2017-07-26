#include <stdlib.h>
#include "threadpool.h"

TPool* initPool (int num) {
  TPool *pool = (TPool *) malloc (sizeof (TPool));
  if (pool == NULL) {
    return NULL;
  }
  pthread_mutex_init(&pool->mutex, NULL);
  pthread_cond_init(&pool->cond, NULL);
  pool->max = num;
  pool->idles = 0;
  pool->total = 0;
  pool->tids = NULL;
  return pool;
}

void releasePool (TPool *pool) {
  while (active_threads(pool) > 0) {
    struct timeval tv = {1,0};
    select (0,0,0,0,&tv);
  }
  while (pool->tids) {
    Thread *th = pool->tids->next;
    pthread_cancel (pool->tids->tid);
    pthread_mutex_lock (&pool->tids->mutex);
    pthread_cond_signal (&pool->tids->cond);
    pthread_mutex_unlock (&pool->tids->mutex);
    pthread_join (pool->tids->tid, NULL);
    pthread_cond_destroy (&pool->tids->cond);
    pthread_mutex_destroy (&pool->tids->mutex);
    free(pool->tids);
    pool->tids = th;
  }
  pthread_cond_destroy (&pool->cond);
  pthread_mutex_destroy (&pool->mutex);
  free (pool);
}

Thread * getWork (TPool *pool) {
  Thread *t = NULL;
  pthread_mutex_lock (&pool->mutex);
  if (pool->idles > 0) {
    --pool->idles;
    t = pool->tids;
    pool->tids = pool->tids->next;
    t->next = NULL;
  } else {
    if (pool->total < pool->max) {
      t = (Thread *) malloc (sizeof (Thread));
      if (t != NULL) {
        pthread_mutex_init(&t->mutex, NULL);
        pthread_cond_init(&t->cond, NULL);
        t->run_flag = 0;
        t->pool = pool;
        t->next = NULL;
        int err = pthread_create (&t->tid, NULL, thread_func, (void*)t);
        if (err) {
          pthread_cond_destroy (&pool->cond);
          pthread_mutex_destroy (&pool->mutex);
          free(t);
          t = NULL;
        } else {
          ++pool->total;
        }
      }
    } else {
      while (pool->idles == 0) {
        pthread_cond_wait (&pool->cond, &pool->mutex);
      }
      --pool->idles;
      t = pool->tids;
      pool->tids = pool->tids->next;
      t->next = NULL;
    }
  }
  pthread_mutex_unlock (&pool->mutex);
  return t;
}

int putWork (TPool *pool, Thread *t) {
  pthread_mutex_lock (&pool->mutex);
  t->next = pool->tids;
  pool->tids = t;
  ++pool->idles;
  if (pool->idles == 1)
    pthread_cond_signal (&pool->cond);
  pthread_mutex_unlock (&pool->mutex);
  return 0;
}

int active_threads (TPool *pool) {
  pthread_mutex_lock (&pool->mutex);
  int actives = pool->total - pool->idles;
  pthread_mutex_unlock (&pool->mutex);
  return actives;
}

void * thread_func (void * arg) {
  Thread *t = (Thread *)arg;
  while (1) {
    pthread_mutex_lock (&t->mutex);
    while (t->run_flag == 0) {
      pthread_testcancel();
      pthread_cond_wait (&t->cond, &t->mutex);
    }
    t->tid_func(t->arg);
    t->run_flag = 0;
    pthread_mutex_unlock (&t->mutex);
    putWork (t->pool, t);
  }
}

int set_work (Thread *t, THD_FUNC func, void *arg) {
  pthread_mutex_lock (&t->mutex);
  t->run_flag = 1;
  t->tid_func = func;
  t->arg = arg;
  pthread_cond_signal (&t->cond);
  pthread_mutex_unlock (&t->mutex);
  return 0;
}
