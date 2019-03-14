#include "httpresponse.h"
#include "unistd.h"
#include "stdio.h"

enum FileType analysePkg(int connfd)
{
    char buff[4096];
    ssize_t len;
    len = read(connfd, buff, sizeof(buff));
    buff[len] = '\0';
    printf("%s", buff);
    return STATIC_RESOURCE;
}

void HttpResponse(int connfd)
{
    enum FileType ft = analysePkg(connfd);
    write(connfd, "HTTP/1.1 200 OK\n", 17);
}