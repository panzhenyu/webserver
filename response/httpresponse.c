#include "httpresponse.h"
#include "unistd.h"
#include "stdio.h"
#include "httprequest.h"

void analysePkg(int connfd)
{
    char buff[4096];
    ssize_t len;
    len = read(connfd, buff, sizeof(buff));
    buff[len] = '\0';
    printf("%s", buff);
}

void HttpResponse(int connfd)
{
    analysePkg(connfd);
    write(connfd, "HTTP/1.1 200 OK\n", 17);
}