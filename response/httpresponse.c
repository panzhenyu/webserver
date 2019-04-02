#include "httpresponse.h"
#include "httprequest.h"
#include "router.h"
#include "bufferedreader.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

static void init_HttpResponse(struct HttpRequest* req, struct HttpResponse *res)
{
    res->header.protocol = req->protocol;
    res->header.kvl = NULL;
    res->body.fp = NULL;
    res->body.msg = NULL;
}

static void response_static(struct HttpResponse *re, int connfd, char *uri)
{
    const struct Router *router = getRouter();
    char *filename = getRoute(router, uri);
    if(filename == NULL)
    {
        re->header.statecode = rsc_404;
        return;
    }
    FILE *fp = fopen(filename, "r");
    if(fp == NULL)
    {
        re->header.statecode = rsc_404;
        printf("!!!!!!!!!File:%s not found\n", filename);
        return;
    }
    re->body.fp = fp;
    fseek(fp, 0, SEEK_END);
    re->body.content_length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
}

static void response(struct HttpResponse *res, int connfd)
{
    char basic[4096];
    sprintf(basic, "HTTP/1.1 200 OK\n");
    sprintf(basic, "%sServer: Tiny Web Server\n", basic);
    sprintf(basic, "%sConnection: close\n", basic);
    sprintf(basic, "%sContent-length: %d\n", basic, res->body.content_length);
    sprintf(basic, "%sContent-type: text/html\n\n", basic);
    write(connfd, basic, strlen(basic));
    if(res->body.fp)
    {
        size_t size;
        FILE *fp = res->body.fp;
        while(size = fread(basic, sizeof(char), sizeof(basic), fp))
            write(connfd, basic, size);
    }
}

void httpResponse_free(struct HttpResponse *res);

void httpResponse(int connfd)
{   
    struct HttpRequest hr;
    struct HttpResponse res;
    analyseHttpRequest(&hr, connfd);
    init_HttpResponse(&hr, &res);
    showHttpRequest(&hr);
    if(hr.req == GET)
        response_static(&res, connfd, hr.uri);
    response(&res, connfd);
    httpRequest_free(&hr);
    // char buff[4096];
    // char content[] = "<html>\n<body>\n111\n</body>\n</html>\n";
    // sprintf(buff, "HTTP/1.1 200 OK\n");
    // sprintf(buff, "%sServer: Tiny Web Server\n", buff);
    // sprintf(buff, "%sConnection: close\n", buff);
    // sprintf(buff, "%sContent-length: %d\n", buff, (int)strlen(content));
    // sprintf(buff, "%sContent-type: text/html\n\n", buff);
    // sprintf(buff, "%s%s", buff, content);
    // write(connfd, buff, strlen(buff));
}
