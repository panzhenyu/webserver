#ifndef _HTTPRESPONSE_H
#define _HTTPRESPONSE_H

#include "httpconfig.h"
#include "keyvaluelist.h"
#include <stdio.h>

enum response_statecode
{
    rsc_100, rsc_101,
    rsc_200, rsc_201, rsc_202, rsc_203, rsc_204, rsc_205, rsc_206,
    rsc_300, rsc_301, rsc_302, rsc_303, rsc_304, rsc_305, rsc_306, rsc_307,
    rsc_400, rsc_401, rsc_402, rsc_403, rsc_404, rsc_405, rsc_406, rsc_407, 
    rsc_408, rsc_409, rsc_410, rsc_411, rsc_412, rsc_413, rsc_414, rsc_415, rsc_416, rsc_417,
    rsc_500, rsc_501, rsc_502, rsc_503, rsc_504, rsc_505, rsc_506, rsc_507, rsc_508, rsc_509, rsc_510
};

struct HttpResponseHeader
{
    enum protocol_type protocol;
    enum response_statecode statecode;
    KeyValueList* kvl;
};

struct HttpResponseBody
{
    FILE *fp;
    char *msg;
    unsigned long content_length;
};

struct HttpResponse
{
    struct HttpResponseHeader header;
    struct HttpResponseBody body;
};

void httpResponse_free(struct HttpResponse *res);
void httpResponse(int connfd);

#endif