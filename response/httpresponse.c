#include <stdio.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "httpresponse.h"
#include "httprequest.h"
#include "router.h"
#include "bufferedreader.h"
#include "keyvaluelist.h"

typedef void (*METHOD) (struct HttpRequest*, struct HttpResponse*);

/* 404處理方案，若static文件夾下有404.html則發送該文件，否則只返回一個404狀態碼 */
static char _404_not_found(struct HttpResponse *res)
{
    HttpResponse_sendStaticFile(res, "./static/404.html");
}

/* 格式化HttpResponse(不包含响应体) */
static char* HttpResponse_toString(struct HttpResponse *res)
{
    int size = 4096, len = 0;
    char *buff = (char*)malloc(size * sizeof(char));
    switch(res->protocol)
    {
        case HTTP_1_0:
            sprintf(buff, "%s", "HTTP/1.0");
            len += 8;
            break;
        case HTTP_1_1:
            sprintf(buff, "%s", "HTTP/1.1");
            len += 8;
            break;
        case HTTP_2_0:
            sprintf(buff, "%s", "HTTP/2.0");
            len += 8;
            break;
    }
    switch(res->statecode)
    {
        case rsc_200:
            sprintf(buff, "%s%s", buff, " 200 OK\n");
            len += 8;
            break;
        case rsc_302:
            sprintf(buff, "%s%s", buff, " 302 Found\n");
            len += 11;
            break;
        case rsc_307:
            sprintf(buff, "%s%s", buff, " 307 Internal Redirect\n");
            len += 23;
            break;
        case rsc_404:
            sprintf(buff, "%s%s", buff, " 404 Not Found\n");
            len += 15;
            break;
        default:
            sprintf(buff, "%s%s", buff, " 500 Server Error\n");
            len += 18;
            break;
    }

    KeyValueList p = res->kvl->next;
    while(p)
    {
        len += strlen(p->key) + strlen(p->value) + 3;
        if(len > size)
        {
            size *= 2;
            buff = realloc(buff, size);
        }
        sprintf(buff, "%s%s: %s\n", buff, p->key, p->value);
        p = p->next;
    }

    if(!KeyValueList_isEmpty(res->cookie))
    {
        len += 12;
        if(len > size)
        {
            size *= 2;
            buff = realloc(buff, size);
        }
        sprintf(buff, "%sSet-Cookie: ", buff);
        p = res->cookie->next;
        while(p)
        {
            len += strlen(p->key) + strlen(p->value) + 2;
            if(len > size)
            {
                size *= 2;
                buff = realloc(buff, size);
            }
            sprintf(buff, "%s%s=%s;", buff, p->key, p->value);
            p = p->next;
        }
        len += 1;
        if(len > size)
        {
            size *= 2;
            buff = realloc(buff, size);
        }
        sprintf(buff, "%s\n", buff);
    }


    len += 2;
    if(len > size)
    {
        size *= 2;
        buff = realloc(buff, size);
    }
    sprintf(buff, "%s\n", buff);
    buff[len-1] = '\0';
    return buff;
}

static void response_static(struct HttpResponse *res, const char *filename)
{
    HttpResponse_sendStaticFile(res, filename);
}

static void response_dynamic(struct HttpRequest *req, struct HttpResponse *res, const char *filename)
{
    void *shared = dlopen(filename, RTLD_LAZY);
    if(!shared)
        _404_not_found(res);
    else
    {
        METHOD m = req->method == GET ? dlsym(shared, "doGet") : dlsym(shared, "doPost");
        if(!m)
            printf("failed to get method");
        else
            m(req, res);
    }
}

/* 初始化HttpResponse */
void HttpResponse_init(struct HttpResponse *res, int connfd)
{
    KeyValueList_init(&res->kvl);
    KeyValueList_append(res->kvl, "Server", "Tiny Web Server");
    KeyValueList_init(&res->cookie);
    res->writer = (struct BufferedWriter*)malloc(sizeof(struct BufferedWriter));
    BufferedWriter_init(res->writer, connfd);
    res->content_length = 0;
}

/* 刷新Response缓存并释放内部资源 */
void HttpResponse_free(struct HttpResponse *res)
{
    KeyValueList_free(res->kvl);
    KeyValueList_free(res->cookie);
    BufferedWriter_free(res->writer);
    free(res->kvl);
    free(res->cookie);
    free(res->writer);
}

/* 设置HttpResponse键值对，若存在就修改，若不存在则添加 */
void HttpResponse_setParameter(struct HttpResponse *res, const char *key, const char *value)
{
    KeyValueList_setValue(res->kvl, key, value);
}

/* 根据key获取value */
const char* HttpResponse_getParameter(const struct HttpResponse *res, const char *key)
{
    return KeyValueList_getValue(res->kvl, key);
}

/* 设置cookie，若存在就修改，若不存在就添加 */
void HttpResponse_setCookie(struct HttpResponse *res, const char *key, const char *value)
{
    KeyValueList_setValue(res->cookie, key, value);
}

/* 根据key获取cookie中的value */
const char* HttpResponse_getCookie(const struct HttpResponse *res, const char *key)
{
    return KeyValueList_getValue(res->cookie, key);
}

/* 设置状态码 */
void HttpResponse_setStateCode(struct HttpResponse *res, enum response_statecode code)
{
    res->statecode = code;
}

/* 设置内容类型 */
void HttpResponse_setContentType(struct HttpResponse *res, enum MIME t)
{
    // char *str = mime2str(t);
    char *str = NULL;
    if(str == NULL)
    {
        str = (char*)malloc(strlen("UNKNOWN") + 1);
        strcpy(str, "UNKNOWN");
    }
    HttpResponse_setParameter(res, "content-type", str);
    free(str);
}

/* 设置内容长度 */
void HttpResponse_setContentLength(struct HttpResponse *res, unsigned long length)
{
    res->content_length = length;
}

/* 请求重定向 */
void HttpResponse_redirect(struct HttpResponse *res, const char *uri)
{
    HttpResponse_setStateCode(res, rsc_302);
    // KeyValueList_append(res->kvl, "Location", uri);
    HttpResponse_setParameter(res, "Location", uri);
    char *msg = HttpResponse_toString(res);
    HttpResponse_sendMsg(res, msg, strlen(msg));
    HttpResponse_sendMsg(res, "\n", 1);
}

/* 请求转发 */
void HttpResponse_transmit(struct HttpResponse *res, const char *uri);

/* 发送静态文件 */
void HttpResponse_sendStaticFile(struct HttpResponse *res, const char *filename)
{
    printf("filename: %s\n", filename);
    int fd;
    FILE *fp;
    char *buff;
    size_t len, rest;
    // 找不到该文件
    if((fp = fopen(filename, "rb")) == NULL)
    {
        res->statecode = rsc_404;
        buff = HttpResponse_toString(res);
        HttpResponse_sendMsg(res, buff, strlen(buff));
        HttpResponse_sendMsg(res, "\n", 1);
        free(buff);
        return;
    }
    // 计算文件长度
    fseek(fp, 0, SEEK_END);
    res->content_length = ftell(fp);
    res->statecode = rsc_200;
    // 发送HttpResponse响应头信息
    buff = HttpResponse_toString(res);
    HttpResponse_sendMsg(res, buff, strlen(buff));
    HttpResponse_flush(res);
    free(buff);
    fclose(fp);

    fd = open(filename, O_RDONLY);
    rest = res->content_length;
    buff = (char*)malloc(4096 * sizeof(char));
    printf("static file size: %ld\n", rest);
    while(rest > 0)
    {
        len = rest > 4096 ? 4096 : rest;
        read(fd, buff, len);
        HttpResponse_sendMsg(res, buff, len);
        rest -= len;
    }
    free(buff);
    close(fd);
    // HttpResponse_sendMsg(res, "<html>Hello!</html>", 20);
    printf("send static file done\n");
}

/* 发送信息 */
void HttpResponse_sendMsg(struct HttpResponse *res, const char *msg, long size)
{
    BufferedWriter_write(res->writer, msg, size);
}

/* 刷新Response缓存 */
void HttpResponse_flush(struct HttpResponse *res)
{
    BufferedWriter_flush(res->writer);
}

/* 响应http请求 */
void response(int connfd)
{
    const struct Map *map;
    struct HttpRequest *req = (struct HttpRequest*)malloc(sizeof(struct HttpRequest));
    struct HttpResponse *res = (struct HttpResponse*)malloc(sizeof(struct HttpResponse));
    analyseHttpRequest(req, connfd);
    showHttpRequest(req);
    HttpResponse_init(res, connfd);
    res->protocol = req->protocol;

    map = getRoute(req->uri);
    if(map)
    {
        if(!strcmp(map->type, "static"))
            response_static(res, map->path);
        else if(!strcmp(map->type, "dynamic"))
            response_dynamic(req, res, map->path);
    }
    else
        printf("cannot find resource: %s\n", req->uri);
    HttpResponse_free(res);
    httpRequest_free(req);
    free(res);
    free(req);
}
