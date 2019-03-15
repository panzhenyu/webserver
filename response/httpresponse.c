#include "httpresponse.h"
#include "httprequest.h"
#include <unistd.h>
#include <stdio.h>

void httpResponse(int connfd)
{   
    struct HttpRequest hr;
    analyseHttpRequest(&hr, connfd);
    showHttpRequest(&hr);
    write(connfd, "HTTP/1.1 200 OK\n", 17);
}
