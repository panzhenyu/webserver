#ifndef _HTTPREQUEST_H
#define _HTTPREQUEST_H

#include "httpconfig.h"
#include "keyvaluelist.h"

struct HttpRequest
{
    enum request_type method;
    enum protocol_type protocol;
    char* uri;
    struct KeyValueNode* kvset;
};

void httpRequest_free(struct HttpRequest *hr);
void analyseHttpRequest(struct HttpRequest *phr, int connfd);
const char* getRequestValueByKey(const struct HttpRequest *phr, const char *key);
void showHttpRequest(const struct HttpRequest *phr);

#endif