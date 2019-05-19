#include "pool.h"
#include "router.h"
#include "internet.h"
#include "clientbuffer.h"
#include "httpresponse.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAXLINE 4096

void client_thread(const void* args)
{
    int connfd;
    pthread_detach(pthread_self());
    while(1)
    {
        connfd = buffer_remove((struct Buffer*)args);
        printf("succeed to get connfd: %d\n");
        response(connfd);
        printf("succeed to respose connfd: %d\n", connfd);
        close(connfd);
    }
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("usage: %s <port>\n", argv[0]);
        exit(0);
    }
    int listenfd = open_listenfd(argv[1]);
    if(listenfd < 0)
        printf("fail to open listenfd\n");
    else
    {
        int connfd;
        struct pool client_handlers;
        struct Buffer buff_clientfd;
        struct sockaddr clientaddr;
        socklen_t clientlen = sizeof(struct sockaddr);
        char client_hostname[MAXLINE], client_port[MAXLINE];
        buffer_init(&buff_clientfd);
        pool_init(&client_handlers, (void*)client_thread, (void*)&buff_clientfd, 5);
        printf("server started\n");
        while(1)
        {
            connfd = accept(listenfd, &clientaddr, &clientlen);
            getnameinfo((const struct sockaddr*)&clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
            // printf("connected to (%s, %s)\n", client_hostname, client_port);
            buffer_insert(&buff_clientfd, connfd);
        }
        // maybe its not necessary to close
        buffer_destroy(&buff_clientfd);
        pool_destroy(&client_handlers);
        close(listenfd);
    }
    return 0;
}
