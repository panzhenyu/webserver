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
    int kvset_len;
    char* uri;
    char* protocol;
    struct KeyValuePair* kvset;
};

void analyseHttpRequest(struct HttpRequest* phr, int connfd);
char* getRequestValueByKey(const struct HttpRequest* phr, const char* key);

#endif