#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "bufferedwriter.h"

/* 初始化writer */
void BufferedWriter_init(struct BufferedWriter *writer, int connfd)
{
    writer->fd = connfd;
    writer->length = 0;
    writer->size = WRITER_BUFF_SIZE;
}

/* 释放writer */
void BufferedWriter_free(struct BufferedWriter *writer)
{
    BufferedWriter_flush(writer);
}

/* 向描述符写size个字节 */
void BufferedWriter_write(struct BufferedWriter *writer, const char *buff, long size)
{
    if(size <= 0)
        return;
    long last = size, pos_len;
    while(last)
    {
        pos_len = writer->size - writer->length;
        pos_len = pos_len > last ? last : pos_len;
        memcpy(writer->buff + writer->length, buff, pos_len);
        writer->length += pos_len;
        if(writer->length == writer->size)
            BufferedWriter_flush(writer);
        last -= pos_len;
    }
}

/* 刷新缓冲区，将未写入的字符全部写入 */
void BufferedWriter_flush(struct BufferedWriter *writer)
{
    write(writer->fd, writer->buff, writer->length);
    writer->length = 0;
}