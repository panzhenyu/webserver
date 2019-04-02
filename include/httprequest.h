#ifndef _HTTPREQUEST_H
#define _HTTPREQUEST_H

#include "httpconfig.h"
#include "keyvaluelist.h"

enum request_type
{
    GET,
    POST
};

struct HttpRequest
{
    enum request_type req;
    enum resource_type res;
    enum protocol_type protocol;
    char* uri;
    struct KeyValueNode* kvset;
};

void httpRequest_free(struct HttpRequest *hr);
void analyseHttpRequest(struct HttpRequest *phr, int connfd);
const char* getRequestValueByKey(const struct HttpRequest *phr, const char *key);
void showHttpRequest(const struct HttpRequest *phr);

#endif