#include "httprequest.h"
#include "bufferedreader.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/* find first character ch from s
   return 0 if s doesn't contain ch */
static int firstChar(const char* s, char ch)
{
    const char *p;
    for(p = s;*p != '\0';p++)
        if(*p == ch)
            return p - s;
    return 0;
}

static void analyseLine(struct HttpRequest* phr, char* s)
{
    int p = 0;
    phr->res = STATIC_RESOURCE;

    // get request method
    p = firstChar(s, ' ');
    s[p] = '\0';
    if(!strcmp(s, "GET"))
        phr->req = GET;
    else
    {
        phr->req = POST;
        phr->res = STATIC_RESOURCE;
    }

    // get uri
    s += p + 1;
    p = firstChar(s, ' ');
    phr->uri = (char*)malloc(sizeof(char) * p);         // needs error handler
    memcpy(phr->uri, s, p);
    if(firstChar(phr->uri, '?'))
        phr->res = DYNAMIC_RESOURCE;

    // get protocol
    s += p + 1;
    p = firstChar(s, '\r');
    phr->protocol = (char*)malloc(sizeof(char) * p);    // needs error handler
    memcpy(phr->protocol, s, p);
}

static char* toLowerCase(const char* s)
{
    char *p = (char*)malloc(sizeof(char) * strlen(s) + 1), ch;
    if(!p)
        return NULL;
    while((ch = *s) != '\0')
    {
        if(ch >= 'A' && ch <= 'Z')
            *p = ch + 32;
        ++p;
    }
    *p = '\0';
    return p;
}

static void analyseHead(struct HttpRequest* phr, char* s)
{
    int idx = firstChar(s, ':');
    s[idx] = '\0';
    
}

void analyseHttpRequest(struct HttpRequest* phr, int connfd)
{
    struct BufferedReader reader;
    bufferedReader_init(&reader, connfd);
    char *p = bufferedReader_nextLine(&reader);
    analyseLine(phr, p);
    // end of httprequest, it would be blocked by read function from bufferedreader without strcmp
    while(strcmp(p = bufferedReader_nextLine(&reader), "\r\n"))
        analyseHead(phr, p);
}

char* getRequestValueByKey(const struct HttpRequest* phr, const char* key);
