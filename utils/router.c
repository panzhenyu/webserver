#include "router.h"
#include <stdlib.h>
#include <string.h>

static struct Router router;

void router_init()
{
    router.map = (KeyValueList)malloc(sizeof(struct KeyValueNode));
    router.map->next = NULL;
    /* begin to init map */
}

const struct Router* getRouter()
{
    return &router;
}

char* getRoute(const struct Router *router, char *uri)
{
    if(!strcmp(uri, "/"))
        return "./index.html";
    char *buff = (char*)malloc(strlen(uri) + 2);
    buff[0] = '.';
    strcpy(buff + 1, uri);
    return buff;
}