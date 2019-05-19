#ifndef _ROUTER_H
#define _ROUTER_H

#include "httpconfig.h"

struct Map{
	char pattern[255];
	char type[255];
	char path[255];
};

const struct Map* getRoute(const char * uri);

#endif
