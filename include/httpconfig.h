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

#endif