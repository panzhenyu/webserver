#ifndef _CLIENTBUFFER_H
#define _CLIENTBUFFER_H

#include <semaphore.h>
#define BUFFER_MAXLEN 100000

struct Buffer
{
    int buff[BUFFER_MAXLEN];
    sem_t consumer;
    sem_t producer;
    sem_t lock;
    int head;
    int tail;
};

void buffer_init(struct Buffer* pbuff);
void buffer_insert(struct Buffer* pbuff, int elem);
int buffer_remove(struct Buffer* pbuff);
void buffer_destroy(struct Buffer* pbuff);

#endif