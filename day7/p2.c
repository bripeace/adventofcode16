#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

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
        char *new_buf = realloc(buf, len+2 * sizeof *buf);
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

typedef struct list {
    void *data;
    struct list *next;
} list;

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


list *get_aba(char *line)
{
    char *p = line;
    bool out_bracket = true;
    list *aba_list = NULL;

    for (;;)
    {
        if (*(p+2) == '\n' || !*p) {
            break;      
        }

        if (*p == '[' || *p == ']') {
            out_bracket = !out_bracket;
        }

        if (!out_bracket || p[1] == '[' || p[2] == '[') {
            p++;
            continue;
        }

        if (p[0] == p[2] && p[0] != p[1]) {
            char *aba = calloc(4, sizeof *aba);
            snprintf(aba, 4, "%.3s",p);
            list *elem = calloc(1, sizeof *elem);
            elem->data = aba;
            aba_list = list_append(aba_list, elem);
        }
        p++;
    }

    return aba_list;
}

bool check_bab(char *line, char *aba)
{
    char *p = line;
    bool out_bracket = true; 
    for (;;) {
        if (*(p+2) == '\n' || !*p) {
            break;
        }

        if (*p == '[' || *p == ']') {
            out_bracket = !out_bracket;
        }

        if (out_bracket || p[1] == ']' || p[2] == ']') {
            p++;
            continue;
        }

        if (p[0] == aba[1] && p[1] == aba[0] && p[1] == aba[2] && p[0] == p[2]) {
            return true;
        }
        p++;
    }

    return false;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage:%s filename\n",argv[0]);
    }

    char *file = argv[1];
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        printf("Could not open %s\n", file);
        return -1;
    }

    int count = 0;
    char *line = NULL;
    while ((line = readline(fp)) != NULL)
    {
        list *aba_list = get_aba(line);
        list *l = aba_list;
        bool ssl = false;
        while (l) {
            char *aba = (char *)l->data;
            ssl = ssl || check_bab(line, aba);
            l = l->next;
        }
        if (ssl) {
            count++;
        }
        list_free(aba_list);
        free(line);
    }
    printf("%d IPs support SSL\n", count);

    fclose(fp);
    return 0;
}
