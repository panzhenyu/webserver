#ifndef _HTTPREQUEST_H
#define _HTTPREQUEST_H

enum request_type
{
    GET,
    POST
};

enum resource_type
{
    STATIC_RESOURCE,
    DYNAMIC_RESOURCE
};

struct KeyValuePair
{
    char* key;
    char* value;
};

struct HttpRequest
{
    enum request_type req;
    enum resource_type res;
    char* uri;
    char* protocol;
    struct KeyValuePair* kvset;
};

struct HttpRequest* analyseHttpRequest(int connfd);
char* getRequestValueByKey(struct HttpRequest* phr, char* key);

#endif