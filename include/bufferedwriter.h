#ifndef _BUFFEREDWRITER_H
#define _BUFFEREDWRITER_H

#define WRITER_BUFF_SIZE 4096

struct BufferedWriter
{
    int fd;
    int length;
    int size;
    char buff[WRITER_BUFF_SIZE];
};

/* 初始化writer */
void BufferedWriter_init(struct BufferedWriter *writer, int connfd);

/* 释放writer */
void BufferedWriter_free(struct BufferedWriter *writer);

/* 向描述符尝试写size个字节 */
void BufferedWriter_write(struct BufferedWriter *writer, const char *buff, long size);

/* 刷新缓冲区，将未写入的字符全部写入 */
void BufferedWriter_flush(struct BufferedWriter *writer);

#endif