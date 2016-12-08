#include "aoc.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#define ROWS 6
#define COLS 50

void print_display (int **display)
{
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            printf ("%s ",display[i][j] == 1 ? "#" : ".");
        }
        printf ("\n");
    }
}

int count_pixels (int **display)
{
    int count =0;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            count += display[i][j];
        }
    }
    return count;
}

// GET RECT SON
// Create a x by y rectangle in upper left corner
bool get_rect(char *args, int **display)
{
    char *p = args;
    while (*p != 'x') {
        p++;
        if (*p == '\n') {
            return false;
        }
    }
    *p++ = 0;
    char *x_str = args;
    args = p;
    while (*p != '\n') {
       p++; 
    }
    *p = 0;
    char *y_str = args;
    
    int xmax = strtoul(x_str, NULL, 0);
    int ymax = strtoul(y_str, NULL, 0);
    assert(xmax <= COLS);
    assert(ymax <= ROWS);
    for (int y = 0; y < ymax; ++y) {
        for (int x = 0; x < xmax; ++x) {
            display[y][x] = 1;
        }
    }

    return true;
}

// args should equal x=[num] by [num]
bool do_rotate_column(char *args, int **display) {
    char *p = args;

    assert(p[0] == 'x');
    errno = 0;
    size_t col = strtoul(&p[2], NULL, 0);
    if (errno != 0) {
        printf ("Problem converting to int: %s\n", &p[2]);
        return false;
    }

    // Skip 2 spaces
    while (*p != ' ') {
        p++;
    }
    p++;
    while (*p != ' ') {
        p++;
    }
    p++;

    size_t offset = strtoul(p, NULL, 0);
    
    if (errno != 0) {
        printf ("Problem converting to int: %s\n", p);
        return false;
    }
    assert(col < COLS); 

    // save layout of column
    int save[ROWS];
    for (size_t i = 0; i < ROWS; ++i) {
        save[i] = display[i][col];
    }
    
    // move layout by offset % ROWS
    for (size_t i = 0; i < ROWS; ++i) {
        int index = (offset+i) % ROWS;
        display[index][col] = save[i]; 
    }

    return true;
}

bool do_rotate_row(char *args, int **display) 
{
    char *p = args;

    assert(p[0] == 'y');
    errno = 0;
    size_t row = strtoul(&p[2], NULL, 0);

    if (errno != 0) {
        printf ("Problem converting to int: %s\n", &p[2]);
        return false;
    }
    assert (row < ROWS);

    // Skip 2 spaces
    while (*p != ' ') {
        p++;
    }
    p++;
    while (*p != ' ') {
        p++;
    }
    p++;

    size_t offset = strtoul(p, NULL, 0);

    if (errno != 0) {
        printf ("Problem converting to int: %s\n", p);
        return false;
    }

    int save[COLS];
    for (size_t i = 0; i < COLS; ++i) {
        save[i] = display[row][i];
    }

    for (size_t i = 0; i < COLS; ++i) {
        int index = (offset+i) % COLS;
        display[row][index] = save[i];
    }

    return true;
}

void run_cmd(char *cmd, char *args, int **display)
{
    printf("\n\n%s %s", cmd, args);
    if (strcmp(cmd, "rotate") == 0) {
        char *p = args;
        while (*p != ' ') {
            p++;
            if (*p == '\n') {
                printf("Malformed input in rotate command\n");
                return;
            }
        } 
        *p++ = 0;
        char *which = args;
        if (strcmp(which, "column") == 0) {
            do_rotate_column(p, display);
        } else if (strcmp(which, "row") == 0) {
            do_rotate_row(p, display);
        }

    } else if (strcmp(cmd, "rect") == 0) {
        if (!get_rect(args, display)) {
            printf ("Malformed input in rect command\n");
            return;
        }
    }

    print_display(display);
}

int main (int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <filename>\n",argv[0]);
        return -1;
    }

    char *file = argv[1];
    FILE *fp = NULL;
    if ((fp = fopen(file, "r")) == NULL) {
        printf("Could not open%s\n",file);
        return -1;
    }

    char *line = NULL;
    int **display = calloc(ROWS, sizeof *display);
    for (int i = 0; i < ROWS; i++) {
        display[i] = calloc(COLS, sizeof(int));
    }

    while ((line = readline(fp))) {
        char *p = line;
        while (*p != ' ') {
            p++; 
        }
        *p++ = 0;
        char *cmd = line;
        char *args = p;
        run_cmd(cmd, args, display);
        free(line);
    }

    int pixels_lit = count_pixels(display);

    printf ("There are %d pixels lit\n", pixels_lit);

    for (int i = 0; i< ROWS; ++i) {
        free(display[i]);
    } 
    free(display);
    fclose(fp);
}

