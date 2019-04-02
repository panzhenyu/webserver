#ifndef _ROUTER_H
#define _ROUTER_H

#include "keyvaluelist.h"

struct Router
{
    KeyValueList map;
};

void router_init();
const struct Router* getRouter();
char* getRoute(const struct Router *router, char *uri);

#endif