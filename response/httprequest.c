#include "httprequest.h"
#include "unistd.h"

struct HttpRequest* analyseHttpRequest(int connfd)
{
}


enum request_type getRequestType(struct HttpRequest* phr);
char* getRequestValueByKey(struct HttpRequest* phr, char* key);