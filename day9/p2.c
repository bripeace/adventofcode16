#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "aoc.h"

unsigned long int decrypt_length(char *str) {
    unsigned long int sum = 0;
    char *p = str;
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
            p++;

            if (*p == '(') {
                    char *sub = calloc(width+1, sizeof *sub);
                    snprintf(sub, width+1, "%s",p);
                    sum += reps * decrypt_length(sub);
                    free(sub);
                    p +=width;
            }  else {
                p += width;
                sum += reps * width;
            }
            continue;
        }
        if (*p != EOF) {
            p++;
            sum++;
        }
    }
    return sum;
}

int main (int argc, char **argv)

{
    if (argc <2) {
        printf("Usage: %s filename\n",argv[0]);
        return -1;
    }

    char *file = argv[1];
    FILE *fp = fopen(file, "r");

    char *line = NULL;
    unsigned long int sum = 0;
    while ((line = readline(fp))) {
        sum = 0;
        int len = strlen(line);
        line[len-1] = 0;
        len--;
        sum += decrypt_length(line);
        printf("Decrypt Lenght: %lu\n",sum);
        free(line);
    }

}

