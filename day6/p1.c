#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

int main(int argc, char *argv[]) 
{
    char *file = NULL;
    if (argc < 2) { 
        printf("Usage: %s filename\n",argv[0]);
        return -1;
    } else {
        file = argv[1];
    }

    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        printf("Could not open file: %s\n", file);
        return -1;

    }

    bool first_line = true; 
    char line[9];
    char *message = NULL;
    int **letters = NULL;
    int num_cols = 0;

    while (fscanf(fp, "%s\n", &line[0]) != EOF) {
        if (first_line) {
            num_cols = strlen(line); 
            first_line = false;
            message = calloc(num_cols+1, sizeof *message);
            letters = calloc(num_cols , sizeof *letters);
            for (int i = 0; i < num_cols; i++) {
                letters[i] = calloc(26, sizeof *letters[i]); 
            }
        }
        char * c = line;
        while (*c) {
            ptrdiff_t col = c-line;
            int index = *c-'a';

            assert(col < num_cols);
            assert(index < 26);

            letters[col][index]++;
            c++;
        }
    }

    int max = 0; 
    for (int c = 0; c < num_cols; c++) {
        max = 0;
        for (int l = 0; l < 26; l++) {
            
            if (letters[c][l] > max)
            {
                max = letters[c][l];
                message[c] = l + 'a';
            }
        }
    }
    printf("%s\n",message);

    return 0;
}
