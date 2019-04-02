#include "bufferedreader.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* init bufferedreader */
void bufferedReader_init(struct BufferedReader* pbr, int fd)
{
    pbr->fd = fd;
    pbr->pos = 0;
    pbr->length = 0;
    pbr->buffsize = BUFFEREDREADER_BUFFSIZE;
}

/* refill the buffer
   return Z* if success
   return -1 if read error occurs
   return 0 if eof */
static int refill(struct BufferedReader* pbr)
{
    pbr->pos = 0;
    pbr->length = read(pbr->fd, pbr->buff, pbr->buffsize);
    return pbr->length;
}

/* get next line from bufferedreader */
char* bufferedReader_nextLine(struct BufferedReader* pbr)
{
    int statcode;
    if(pbr->pos >= pbr->length)         // generally pbr->pos will nerver bigger than pbr->length, add a error handler must be better
    {
        statcode = refill(pbr);
        if(statcode == 0)
            return NULL;
        else if(statcode == -1)
        {
            // needs error handler
            return NULL;
        }
    }

    char* ret = NULL;
    int pbrpos = pbr->pos;
    unsigned int length = 0, increment = 0;
    while(pbr->buff[pbrpos++] != '\n')
    {
        if(pbrpos == pbr->length)
        {
            increment = pbr->length - pbr->pos;
            ret = realloc(ret, length + increment + 2);     // needs error handler
            memcpy(ret + length, pbr->buff + pbr->pos, increment * sizeof(char));
            length += increment;
            statcode = refill(pbr);
            if(statcode == 0 || statcode == -1)
            {
                ret[length] = '\n';
                ret[length + 1] = '\0';
                return ret;
            }
            else if(statcode == -1)
            {
                // needs error handler
                ret[length] = '\n';
                ret[length + 1] = '\0';
                return ret;
            }
            pbrpos = 0;
        }
    }

    increment = pbrpos - pbr->pos;
    if(increment)
    {
        ret = realloc(ret, length + increment + 1);
        memcpy(ret + length, pbr->buff + pbr->pos, increment * sizeof(char));
        length += increment;
        ret[length] = '\0';
        pbr->pos = pbrpos;
    }
    return ret;
}
