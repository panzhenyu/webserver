#include "bufferedreader.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"

/* init bufferedreader */
void bufferedReader_init(struct BufferedReader* pbr, int fd)
{
    pbr->fd = fd;
    pbr->pos = 0;
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
    char* ret = NULL;
    unsigned int oldlen, poslen = 0;
    int pbr_oldpos;
    if(pbr->length == 0 || pbr->pos == pbr->length - 1)
    {
        pbr_oldpos = refill(pbr);
        switch (pbr_oldpos)
        {
            case -2:
                return NULL;
            case -1:
                return NULL;                    // needs error handler
            default:
                break;
        }
    }


    int pbrpos = pbr->pos;
    while(pbr->buff[pbrpos++] != '\n')
    {
        --pbr->rest_len;
        if(pbr->rest_len == 0)
        {
            oldlen = poslen;
            poslen += pbr->buffsize - pbr->pos;
            ret = realloc(ret, poslen);                             // needs error handler
            memcpy(ret[oldlen], pbr->buff[pbr->pos], )
        }
    }
}