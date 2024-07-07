#include<internal/thread_pool.h>

ThreadPool thread_pool_create(size_t num_threads)
{
    if(num_threads <= 0 || num_threads > MAX_TP_THREADS)return NULL;

    ThreadPool tp = malloc(sizeof(ThreadPool_t));
    tp->thread_count = num_threads;
    tp->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    tp->cond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    tp->threads = malloc(sizeof(pthread_t) * num_threads);
    tp->quit = false;
    VECTOR_INIT(tp->work_v);
    for(size_t i = 0; i < num_threads; i++)
    {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tp->threads[i], &attr, (void*(*)(void*))thread_pool_worker, tp);
    }

    return tp;
}

void thread_pool_enqueue(ThreadPool tp, void*(*func)(void*), void* arg)
{
    pthread_mutex_lock(&tp->mutex);
    ThreadWork_t work = {.func = func, .arg = arg};
    VECTOR_PUSH(tp->work_v, work);
    pthread_mutex_unlock(&tp->mutex);

    pthread_cond_signal(&tp->cond);
}

void thread_pool_destroy(ThreadPool tp)
{
    pthread_mutex_lock(&tp->mutex);
    tp->quit = true;
    pthread_mutex_unlock(&tp->mutex);

    pthread_cond_broadcast(&tp->cond);

    for(size_t i = 0; i < tp->thread_count; i++)
    {
        pthread_join(tp->threads[i], NULL);
    }

    VECTOR_FREE(tp->work_v);
    free(tp->threads);
    pthread_cond_destroy(&tp->cond);
    pthread_mutex_destroy(&tp->mutex);
    free(tp);
}

void* thread_pool_worker(ThreadPool tp)
{
    ThreadWork_t tw;

    while(true)
    {
        pthread_mutex_lock(&tp->mutex);
        while(VECTOR_EMPTY(tp->work_v) && !tp->quit)pthread_cond_wait(&tp->cond, &tp->mutex);

        if(tp->quit && VECTOR_EMPTY(tp->work_v))
        {
            pthread_mutex_unlock(&tp->mutex);
            return NULL;
        }

        tw = VECTOR_POPF(tp->work_v);
        pthread_mutex_unlock(&tp->mutex);

        tw.func(tw.arg);
    }
}
