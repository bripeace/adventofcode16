#include "aoc.h"
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

// Read in one line including \n
// User must free() pointer
// file positon will be restored on failure 
char *readline(FILE *fp)
{
    if (fp == NULL || feof(fp)) {
        return NULL;
    }

    long int restore = ftell(fp);

    size_t buf_size = 100;
    char *buf = calloc(buf_size, sizeof *buf);
    char *p = buf;
    size_t len = 0;
    char c;

    while (fread(&c, sizeof c, 1, fp)==1) {
        if (c == EOF || c == '\n') {
            *p++ = '\n';
            *p = 0;
            break;
        }

        // Grow buffer if too small +2 = \n0
        if (len+2 >= buf_size) {
            if (buf_size > UINT_MAX - 100) {
                // overflow
                errno = ENOMEM;
                goto fail;
            }
            buf_size +=100;
            if (buf_size > UINT_MAX / 100)
            {
                //overflow 
                errno = ENOMEM;
            }
            char *new_buf = realloc(buf, buf_size * sizeof *buf);
            if (new_buf != NULL) {
                buf = new_buf;
                p = buf + len;
            } else {
                goto fail;
            }
        }

        *p++=c;
        len++;
    }

    if (len == 0)
        goto fail;

    //shrink buf to only the size we used 
    //include space for \n and 0
    if (len+2 < buf_size) {
        char *new_buf = realloc(buf, (len+2) * sizeof *buf);
        if (new_buf == NULL) {
            goto fail;
        }            
        buf = new_buf;
    }
    return buf;
    // We failed somewhere
fail:
    free(buf);
    fseek(fp, restore, SEEK_SET);  
    return NULL;
}

list *list_append(list *nlist, list *elem)
{
    if (nlist == NULL) {
        assert(elem != NULL);
        return elem;
    }

    list *l = nlist;
    for (;;) {
        if (l->next == NULL) {
            l->next = elem;
            break;
        }
        l = l->next;
    }
    return nlist;
}

void list_free(list *nlist) 
{
    list *l = nlist;
    while(l) {

        list *next = l->next;
        free(l->data);
        free(l);
        if (next == NULL) {
            break;
        }
        l = next;
    }
}
