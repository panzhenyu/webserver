#ifndef _POOL_H
#define _POOL_H

#include <pthread.h>

struct pool
{
    void* function;
    pthread_t* handlers;
    int thread_count;
};

void pool_init(struct pool* ppool, void* func, void* args, int num);
void pool_destroy(struct pool* ppool);

#endif