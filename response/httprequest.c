#include "httprequest.h"
#include "bufferedreader.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

void httpRequest_free(struct HttpRequest *hr)
{
    free(hr->uri);
    struct KeyValueNode *t;
    while(hr->kvset)
    {
        t = hr->kvset;
        hr->kvset = hr->kvset->next;
        free(t);
    }
}

/*
   find first character ch from s
   return -1 if s doesn't contain ch
   or return index of ch
*/
static int firstChar(const char* s, char ch)
{
    if(!s)
        return 0;
    const char *p;
    for(p = s;*p != '\0';p++)
        if(*p == ch)
            return p - s;
    return -1;
}

static void analyseLine(struct HttpRequest* phr, char* s)
{
    if(!phr || !s)
        return;
    char *fs = s;
    int p = 0;

    // get request method
    p = firstChar(s, ' ');
    s[p] = '\0';
    if(!strcmp(s, "GET"))
        phr->method = GET;
    else
        phr->method = POST;

    // get uri
    s = &s[p + 1];
    p = firstChar(s, ' ');
    s[p] = '\0';
    phr->uri = (char*)malloc(sizeof(char) * (p + 1));         // needs error handler
    strcpy(phr->uri, s);

    // get protocol
    s = &s[p + 1];
    p = firstChar(s, '\r');
    s[p] = '\0';
    if(strcmp(s, "HTTP/1.1"))
        phr->protocol = HTTP_1_1;
    else if(strcmp(s, "HTTP/1.0"))
        phr->protocol = HTTP_1_0;
    else if(strcmp(s, "HTTP/2.0"))
        phr->protocol = HTTP_2_0;
    else
        phr->protocol = UNKNOWN_PROTOCOL;
    free(fs);
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

static void analyseHead(struct KeyValueNode* pkv, char* s)
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
    while(s[idx] == ' ')
        idx++;
    int len = firstChar(s + idx, '\r') + 1;
    pkv->value = (char*)malloc(sizeof(char) * len);     // needs error handler
    memcpy(pkv->value, s + idx, len - 1);
    pkv->value[len - 1] = '\0';
    free(s);
}

static void httpRequest_init(struct HttpRequest* phr)
{
    phr->uri = NULL;
    phr->kvset = (struct KeyValueNode*)malloc(sizeof(struct KeyValueNode));     // needs error handler
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

    struct KeyValueNode* pkv = phr->kvset;
    // end of httprequest, it would be blocked by read function from bufferedreader without strcmp
    while(strcmp(p = bufferedReader_nextLine(&reader), "\r\n"))
    {
        pkv->next = (struct KeyValueNode*)malloc(sizeof(struct KeyValueNode));  // needs error handler
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
    const struct KeyValueNode* pkv = phr->kvset->next;
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
    switch(phr->method)
    {
        case GET:
            printf("request-method: GET\n");
            break;
        case POST:
            printf("request-method: POST\n");
            break;
    }
    printf("uri: %seof\n", phr->uri);
    switch(phr->protocol)
    {
        case HTTP_1_0:
            printf("protocol: HTTP/1.0\n");
            break;
        case HTTP_1_1:
            printf("protocol: HTTP/1.1\n");
            break;
        case HTTP_2_0:
            printf("protocol: HTTP/2.0\n");
            break;
        default:
            printf("protocol: unknown\n");
            break;
    }
    struct KeyValueNode* pkv = phr->kvset->next;    // skip the head node
    while(pkv)
    {
        printf("%s: %s\n", pkv->key, pkv->value);
        pkv = pkv->next;
    }
    sem_post(&print_lock);
}
