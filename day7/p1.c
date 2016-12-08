#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

// Read in a one line including \n
// User must free() pointer
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

    //shrink buf to only the size we used count \n and 0 which is not in len
    if (len+2 < buf_size) {
        char *new_buf = realloc(buf, len+2 * sizeof *buf);
        if (new_buf == NULL) {
            goto fail;
        }            
        buf = new_buf;
        
    }
    return buf;
    printf("\n");
    // We failed somewhere
fail:
    free(buf);
    fseek(fp, restore, SEEK_SET);  
    return NULL;
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
        
        char *p = line;
        bool out_bracket = true;
        bool tls = true;
        bool found = false;
        for ( ;; )
        {
            if (*p == '\n') {
              break;      
            }

            if (*p == '[' || *p == ']') {
                out_bracket = !out_bracket;
            }
           
            if (p[0] == p[3] && p[1] == p[2] && p[0] != p[1]) {
                   found = true; 
                   tls = tls && out_bracket;
                   printf("found %.4s %s\t\t%c%c%c%c\n", p,  tls ? "true" : "false", p[0], p[1], p[2],p[3]);

            }
            p++;
        }

        line[strlen(line)-1] = 0;

        if (tls && found) {
            printf("%s %s\n\n",line, tls ? "true" : "false");       
            count++;
        }
        free(line);
    }
    printf("%d IPs support TLS\n", count);

    fclose(fp);
    return 0;
}
