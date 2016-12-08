#pragma once
#include <stdio.h>

typedef struct list {
        void *data;
        struct list *next;
} list;

char *readline(FILE *fp);
list *list_append(list *nlist, list *elem);
void list_free(list *nlist);
