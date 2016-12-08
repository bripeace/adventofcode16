#include "aoc.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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
