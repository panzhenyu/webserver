#include "httpresponse.h"
#include "httprequest.h"
#include <unistd.h>

void HttpResponse(int connfd)
{
    write(connfd, "HTTP/1.1 200 OK\n", 17);
}