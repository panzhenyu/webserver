#ifndef _HTTPCONFIG_H
#define _HTTPCONFIG_H

enum protocol_type
{
    HTTP_1_0,
    HTTP_1_1,
    HTTP_2_0,
    UNKNOWN_PROTOCOL
};

enum resource_type
{
    STATIC_RESOURCE,
    DYNAMIC_RESOURCE
};

#define CONTENT_TYPE_HTML "text/html"
#define CONTENT_TYPE_CSS 
#define CONTENT_TYPE_JAVASCRIPT
#define CONTENT_TYPE_IMAGE "image"

#endif