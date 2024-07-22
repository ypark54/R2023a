/* Copyright 2022 The MathWorks, Inc. */
/*!
 * @file
 * pthread support for codegen, see basker_pthread_support.cpp for cpp implementation and tests
 */

#ifndef nesl_thread_h
#define nesl_thread_h

#include <mc_basker_thread.h>
#include <nesl_rtw.h>
#include <pthread.h>

typedef struct {
    size_t          count;
    size_t          num;
    pthread_mutex_t count_mutex;
    pthread_cond_t  ok_to_proceed;
} tbarrier_t;

PMF_DEPLOY_STATIC void local_barrier_init(tbarrier_t* b, size_t num) {
    b->count = 0;
    b->num   = num;
    pthread_mutex_init(&(b->count_mutex), NULL);
    pthread_cond_init(&(b->ok_to_proceed), NULL);
}

PMF_DEPLOY_STATIC void local_barrier_wait(tbarrier_t* b) {
    pthread_mutex_lock(&(b->count_mutex));

    b->count++;
    if (b->count == b->num) {
        b->count = 0;
        pthread_cond_broadcast(&(b->ok_to_proceed));
    } else {
        pthread_cond_wait(&(b->ok_to_proceed), &(b->count_mutex));
    }

    pthread_mutex_unlock(&(b->count_mutex));
}

PMF_DEPLOY_STATIC void local_barrier_destroy(tbarrier_t* b) {
    pthread_mutex_destroy(&(b->count_mutex));
    pthread_cond_destroy(&(b->ok_to_proceed));
}

struct McBaskerThreadTag {
    pthread_t*      threads;
    pthread_mutex_t mutex;
    tbarrier_t      start_barrier;
    tbarrier_t      end_barrier;
    size_t          cnt;
    size_t          num;
    bool            stop;
    void*           mArg;
    ThreadFunc      mFcn;
};

/* worker in each thread */
PMF_DEPLOY_STATIC void* pthread_worker(void* arg) {
    McBaskerThread* tm = (McBaskerThread*)arg;
    ThreadFuncArg   fcn_arg;

    while (1) {
        local_barrier_wait(&(tm->start_barrier));

        if (tm->stop) {
            break;
        }

        pthread_mutex_lock(&(tm->mutex));

        fcn_arg.mArg = tm->mArg;
        fcn_arg.mId  = tm->cnt;

        tm->cnt++;

        pthread_mutex_unlock(&(tm->mutex));

        tm->mFcn((void*)&fcn_arg);

        local_barrier_wait(&(tm->end_barrier));
    }

    local_barrier_wait(&(tm->end_barrier));

    return NULL;
}

/* create a thread pool with num threads. One thread has one worker. */
PMF_DEPLOY_STATIC McBaskerThread* create_pthread(ThreadFunc fcn, size_t num) {
    McBaskerThread* tm   = (McBaskerThread*)malloc(sizeof(McBaskerThread));
    pthread_t*      tids = (pthread_t*)malloc(num * sizeof(pthread_t));
    pthread_t       thread;
    size_t          i;

    tm->cnt  = 0;
    tm->num  = num;
    tm->stop = false;
    tm->mArg = NULL;
    tm->mFcn = fcn;

    pthread_mutex_init(&(tm->mutex), NULL);
    local_barrier_init(&(tm->start_barrier), num + 1);
    local_barrier_init(&(tm->end_barrier), num + 1);

    for (i = 0; i < num; i++) {
        pthread_create(&thread, NULL, pthread_worker, tm);
        tids[i] = thread;
    }
    tm->threads = tids;

    return tm;
}

/* trigger workers in all threads, wait until all threads finish. */
PMF_DEPLOY_STATIC void pthread_post(McBaskerThread* tm) {
    local_barrier_wait(&(tm->start_barrier));

    local_barrier_wait(&(tm->end_barrier));
}

PMF_DEPLOY_STATIC int run_pthread(McBaskerThread* tm, void* arg) {
    tm->cnt  = 0;
    tm->stop = false;
    tm->mArg = arg;

    pthread_post(tm);

    return 0;
}

PMF_DEPLOY_STATIC void destroy_pthread(McBaskerThread* tm) {
    size_t i;

    /* send stop signal to all threads */
    tm->stop = true;

    pthread_post(tm);

    /* wait until all threads exit */
    for (i = 0; i < tm->num; i++) {
        pthread_join(tm->threads[i], NULL);
    }

    /* release resource */
    free(tm->threads);

    pthread_mutex_destroy(&(tm->mutex));
    local_barrier_destroy(&(tm->end_barrier));
    local_barrier_destroy(&(tm->start_barrier));

    free(tm);
}

/*
 * pthread implementation
 */
PMF_DEPLOY_STATIC void register_pthread_support(void) {
    static McBaskerThreadMethods s_methods;

    s_methods.mCreateThreadMethod  = &create_pthread;
    s_methods.mRunThreadMethod     = &run_pthread;
    s_methods.mDestroyThreadMethod = &destroy_pthread;

    mc_register_thread_support(&s_methods);
}

#endif /* include guard */
