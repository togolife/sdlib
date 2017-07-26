#include <pthread.h>

typedef struct ThreadPool TPool;

typedef struct Thread {
  pthread_t tid;
  TPool *pool;
  struct Thread *next;
  void * (*tid_func) (void *);
  void *arg;
  pthread_mutex_t  mutex;
  pthread_cond_t   cond;
  int run_flag;
} Thread;

typedef struct ThreadPool {
  int max;   // 最大线程数
  int total; // 已申请线程数
  int idles; // 当前空闲线程数
  struct Thread *tids; // 空闲线程链表
  pthread_mutex_t  mutex;
  pthread_cond_t   cond;
} TPool;

typedef void * (*THD_FUNC) (void *);
void * thread_func(void *);
TPool* initPool (int num);
void releasePool (TPool *pool);
Thread * getWork (TPool *pool);
int putWork (TPool *pool, Thread *t);
int active_threads (TPool *pool);
int set_work (Thread *t, THD_FUNC func, void *arg);
