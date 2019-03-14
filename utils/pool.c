#include "pool.h"
#include <stdlib.h>
#include <stdio.h>

void pool_init(struct pool* ppool, void* func, void* args, int num)
{
    ppool->function = func;
    ppool->thread_count = num;
    ppool->handlers = (pthread_t*)malloc(ppool->thread_count * sizeof(pthread_t));
    if(!ppool->handlers)
    {
        printf("fail to malloc ppool->handler\n");
        exit(0);
    }
    int i;
    pthread_t id;
    for(i = 0; i < ppool->thread_count; ++i)
    {
        pthread_create(&id, NULL, ppool->function, args);
        ppool->handlers[i] = id;
    }
}

void pool_destroy(struct pool* ppool)
{
    free(ppool->handlers);
    ppool->thread_count = -1;
    ppool->function = NULL;
}