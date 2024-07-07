#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H
#include<stdlib.h>
#include<stdbool.h>

/**
 * Maximum number of threads for a thread pool.
 */
#define MAX_TP_THREADS 64

/**
 * ThreadPool type for efficient async operations.
 */
typedef struct ThreadPool_t* ThreadPool;

/**
 * Creates a thread pool with a given number of threads.
 * The threads are started automatically but this is not a problem.
 * 
 * \param num_threads maximum number of threads in parallel
 * \returns A thread pool handle or NULL on failure.
 */
ThreadPool thread_pool_create(size_t num_threads);
/**
 * Adds work to the given ThreadPool.
 * 
 * \param tp the ThreadPool
 * \param func the thread function to run
 * \param arg the thread argument
 */
void thread_pool_enqueue(ThreadPool tp, void*(*func)(void*), void* arg);
/**
 * Destroy a given thread pool while waiting for all threads to finish.
 * 
 * \param tp the thread pool to destroy
 */
void thread_pool_destroy(ThreadPool tp);

#endif /* _THREAD_POOL_H */