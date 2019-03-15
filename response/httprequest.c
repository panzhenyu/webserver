#include "httprequest.h"
#include "bufferedreader.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

/* find first character ch from s
   return 0 if s doesn't contain ch */
static int firstChar(const char* s, char ch)
{
    if(!s)
        return 0;
    const char *p;
    for(p = s;*p != '\0';p++)
        if(*p == ch)
            return p - s;
    return 0;
}

static void analyseLine(struct HttpRequest* phr, char* s)
{
    if(!phr || !s)
        return;
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

static void toLowerCase(char* s)
{
    if(!s)
        return;
    char ch;
    while((ch = *s) != '\0')
    {
        if(ch >= 'A' && ch <= 'Z')
            *s = ch + 32;
        ++s;
    }
}

static void analyseHead(struct KeyValuePair* pkv, char* s)
{
    if(!pkv || !s)
        return;
    // get key
    int idx = firstChar(s, ':');
    s[idx] = '\0';
    pkv->key = (char*)malloc(sizeof(char) * idx);       // needs error handler
    strcpy(pkv->key, s);
    toLowerCase(pkv->key);

    idx++;
    while(s[idx++] != ' ');
    int len = firstChar(s + idx, '\r') + 1;
    pkv->value = (char*)malloc(sizeof(char) * len);     // needs error handler
    memcpy(pkv->value, s + idx, len - 1);
    pkv->value[len - 1] = '\0';
}

static void httpRequest_init(struct HttpRequest* phr)
{
    phr->uri = phr->protocol = NULL;
    phr->kvset = (struct KeyValuePair*)malloc(sizeof(struct KeyValuePair));     // needs error handler
}

void analyseHttpRequest(struct HttpRequest* phr, int connfd)
{
    if(!phr)
        return;
    httpRequest_init(phr);
    struct BufferedReader reader;
    bufferedReader_init(&reader, connfd);
    char *p = bufferedReader_nextLine(&reader);
    analyseLine(phr, p);

    struct KeyValuePair* pkv = phr->kvset;
    // end of httprequest, it would be blocked by read function from bufferedreader without strcmp
    while(strcmp(p = bufferedReader_nextLine(&reader), "\r\n"))
    {
        pkv->next = (struct KeyValuePair*)malloc(sizeof(struct KeyValuePair));  // needs error handler
        pkv = pkv->next;
        pkv->next = NULL;
        analyseHead(pkv, p);
    }
}

const char* getRequestValueByKey(const struct HttpRequest* phr, const char* key)
{
    if(!phr || !key)
    {
        // needs error handler
        return NULL;
    }
    char* temp = (char*)malloc(strlen(key) * sizeof(char) + 1);
    strcpy(temp, key);
    toLowerCase(temp);
    const struct KeyValuePair* pkv = phr->kvset->next;
    while(pkv && strcmp(pkv->key, temp))
        pkv = pkv->next;
    free(temp);
    return pkv ? pkv->value : NULL;
}

static sem_t print_lock;
static pthread_once_t pot = PTHREAD_ONCE_INIT;
static void init_lock()
{
    sem_init(&print_lock, 0, 1);
}

void showHttpRequest(const struct HttpRequest* phr)
{
    pthread_once(&pot, init_lock);
    sem_wait(&print_lock);
    switch(phr->req)
    {
        case GET:
            printf("request-method: GET\n");
            break;
        case POST:
            printf("request-method: POST\n");
            break;
    }
    switch(phr->res)
    {
        case STATIC_RESOURCE:
            printf("resource-type: static\n");
            break;
        case DYNAMIC_RESOURCE:
            printf("resource-type: dynamic\n");
            break;
    }
    printf("uri: %s\n", phr->uri);
    printf("protocol: %s\n", phr->protocol);
    struct KeyValuePair* pkv = phr->kvset->next;    // skip the head node
    while(pkv)
    {
        printf("%s: %s\n", pkv->key, pkv->value);
        pkv = pkv->next;
    }
    sem_post(&print_lock);
}
