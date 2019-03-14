#ifndef _BUFFEREDREADER_H
#define _BUFFEREDREADER_H

#define BUFFEREDREADER_BUFFSIZE 4096

struct BufferedReader
{
    char buff[BUFFEREDREADER_BUFFSIZE];
    int pos;
    int fd;
    int length;
    int buffsize;
};

void bufferedReader_init(struct BufferedReader* pbr, int fd);
char* bufferedReader_nextLine(struct BufferedReader* pbr);

#endif