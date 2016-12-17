#include <stdio.h>
/*
#define INPUT_SIZE 2
char *input[INPUT_SIZE] = {
    "Disc #1 has 5 positions; at time=0, it is at position 4.",
    "Disc #2 has 2 positions; at time=0, it is at position 1."
};
*/

#define INPUT_SIZE 6
char *input[INPUT_SIZE] = {
    "Disc #1 has 17 positions; at time=0, it is at position 5.",
    "Disc #2 has 19 positions; at time=0, it is at position 8.",
    "Disc #3 has 7 positions; at time=0, it is at position 1.",
    "Disc #4 has 13 positions; at time=0, it is at position 7.",
    "Disc #5 has 5 positions; at time=0, it is at position 1.",
    "Disc #6 has 3 positions; at time=0, it is at position 0.",
};

struct disc {
    int positions;
    int start;
};

int main (void)
{

    // process input
    struct disc discs[INPUT_SIZE];
    for (int i = 0; i < INPUT_SIZE; i++) {
        sscanf(input[i], "Disc #%*d has %d positions; at time=0, it is at position %d", &discs[i].positions, &discs[i].start);
    }

    int t = 0;
    for (;;) {
        int s = 0;
 //       printf ("t=%d\n",t);
        for (int i = 0; i < INPUT_SIZE; i++) {
            s += (discs[i].start + t+i + 1) % discs[i].positions;
//            printf ("Disc %d at %d at %d\n",i, (discs[i].start + t+i+1) % discs[i].positions, t+i+1);
            
        }
        if (!s) {
             break;
        }
        t++;
    }
    printf ("t=%d\n",t);
}

