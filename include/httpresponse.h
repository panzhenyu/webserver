#ifndef _HTTPRESPONSE_H
#define _HTTPRESPONSE_H

#include "httpconfig.h"
#include "httprequest.h"
#include "keyvaluelist.h"
#include "bufferedwriter.h"
#include "mime.h"

#define RESPONSE_BUFF_INITSIZE 4096

struct HttpResponse
{
    enum protocol_type protocol;        // HTTP协议版本号
    enum response_statecode statecode;  // HTTP状态码
    KeyValueList kvl;                   // 响应头键值对
    KeyValueList cookie;                // cookie
    unsigned long content_length;       // 响应体大小
    struct BufferedWriter *writer;      // 响应器
};

/* 初始化HttpResponse */
void HttpResponse_init(struct HttpResponse *res, int connfd);

/* 刷新Response缓存并释放内部资源 */
void HttpResponse_free(struct HttpResponse *res);

/* 响应http请求 */
void response(int connfd);

/* 设置HttpResponse键值对，若存在就修改，若不存在则添加 */
void HttpResponse_setParameter(struct HttpResponse *res, const char *key, const char *value);

/* 根据key获取value */
const char* HttpResponse_getParameter(const struct HttpResponse *res, const char *key);

/* 设置cookie，若存在就修改，若不存在就添加 */
void HttpResponse_setCookie(struct HttpResponse *res, const char *key, const char *value);

/* 根据key获取cookie中的value */
const char* HttpResponse_getCookie(const struct HttpResponse *res, const char *key);

/* 设置状态码 */
void HttpResponse_setStateCode(struct HttpResponse *res, enum response_statecode code);

/* 设置内容类型 */
void HttpResponse_setContentType(struct HttpResponse *res, enum MIME t);

/* 设置内容长度 */
void HttpResponse_setContentLength(struct HttpResponse *res, unsigned long length);

/* 请求重定向 */
void HttpResponse_redirect(struct HttpResponse *res, const char *uri);

/* 请求转发 */
void HttpResponse_transmit(struct HttpResponse *res, const char *uri);

/* 发送静态文件 */
void HttpResponse_sendStaticFile(struct HttpResponse *res, const char *filename);

/* 发送信息 */
void HttpResponse_sendMsg(struct HttpResponse *res, const char *msg, long size);

/* 刷新Response缓存 */
void HttpResponse_flush(struct HttpResponse *res);

#endif