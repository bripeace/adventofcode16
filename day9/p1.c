#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "aoc.h"

int main (int argc, char **argv)
{

    if (argc <2) {
        printf("Usage: %s filename\n",argv[0]);
        return -1;
    }

    char *file = argv[1];
    FILE *fp = fopen(file, "r");

    char *line = NULL;
    int sum = 0;
    while ((line = readline(fp))) {
        sum = 0;
        int len = strlen(line);
        line[len-1] = 0;
        len--;
        char *p = line;
        printf("%s - ",p);
        while (*p) {
            if (*p == '(') {
                int width = strtoul(&p[1], NULL, 0);
                while (*p != 'x') {
                    p++;
                }

                int reps = strtoul(&p[1], NULL, 0);
                while (*p != ')') {
                    p++;
                }

                sum += width * reps;
                p += 1 + width;
                continue;
            }
            p++;
            sum++;
        }
        printf("%d\n",sum);
        free(line);
    }

}

