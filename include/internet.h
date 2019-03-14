#ifndef _INTERNET_H
#define _INTERNET_H

int open_clientfd(char *hostname, char *port);
int open_listenfd(char *port);

#endif