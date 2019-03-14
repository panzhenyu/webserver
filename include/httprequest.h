#ifndef _HTTPREQUEST_H
#define _HTTPREQUEST_H

enum request_type
{
    GET,
    POST
};

struct JsonPackage
{

};

struct HttpRequest
{
    enum request_type rt;
    
};

struct HttpRequest analyseHttpRequest(int connfd);

#endif