#ifndef _THREAD_POOL_H
#include<thread_pool.h> // Defines _THREAD_POOL_H
#include<pthread.h>
#include<internal/core.h>

/**
 * Thread work.
 */
typedef struct {
    void* (*func)(void*);
    void* arg;
} ThreadWork_t;

/**
 * Vector of ThreadWork_t for ThreadPool.
 */
DEFINE_VECTOR_OF(ThreadWork_t) ThreadWork_v;

/**
 * Implementation of ThreadPool type.
 */
typedef struct ThreadPool_t {
    pthread_t* threads;
    size_t thread_count;
    ThreadWork_v work_v;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    bool quit;
} ThreadPool_t;

/**
 * Thread pool worker function. Not meant to be called directly.
 * 
 * \param tp the thread pool
 * \returns I do not know. I will just return NULL.
 */
void* thread_pool_worker(ThreadPool tp);

#endif /* _THREAD_POOL_H */