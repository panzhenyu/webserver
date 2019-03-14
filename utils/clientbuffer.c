#include "clientbuffer.h"
#include <unistd.h>

void buffer_init(struct Buffer* pbuff)
{
    pbuff->head = pbuff->tail = 0;
    sem_init(&pbuff->consumer, 0, 0);
    sem_init(&pbuff->producer, 0, BUFFER_MAXLEN);
    sem_init(&pbuff->lock, 0, 1);
}

void buffer_insert(struct Buffer* pbuff, int elem)
{
    sem_wait(&pbuff->producer);
    sem_wait(&pbuff->lock);
    pbuff->buff[pbuff->tail++] = elem;
    pbuff->tail %= BUFFER_MAXLEN;
    sem_post(&pbuff->lock);
    sem_post(&pbuff->consumer);
}

int buffer_remove(struct Buffer* pbuff)
{
    sem_wait(&pbuff->consumer);
    sem_wait(&pbuff->lock);
    int fd = pbuff->buff[pbuff->head++];
    pbuff->head %= BUFFER_MAXLEN;
    sem_post(&pbuff->lock);
    sem_post(&pbuff->producer);
    return fd;
}

void buffer_destroy(struct Buffer* pbuff)
{
    while(pbuff->head != pbuff->tail)
    {
        close(pbuff->buff[pbuff->head++]);
        pbuff->head %= BUFFER_MAXLEN;
    }
}
