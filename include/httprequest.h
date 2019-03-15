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
    struct KeyValuePair* next;
};

struct HttpRequest
{
    enum request_type req;
    enum resource_type res;
    char* uri;
    char* protocol;
    struct KeyValuePair* kvset;     // the head node is empty
};

void analyseHttpRequest(struct HttpRequest* phr, int connfd);
const char* getRequestValueByKey(const struct HttpRequest* phr, const char* key);
void showHttpRequest(const struct HttpRequest* phr);

#endif