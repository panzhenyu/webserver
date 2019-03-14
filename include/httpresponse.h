#ifndef _HTTPRESPONSE_H
#define _HTTPRESPONSE_H

enum FileType
{
    STATIC_RESOURCE,
    DYNAMIC_RESOURCE
};

void HttpResponse(int connfd);

#endif