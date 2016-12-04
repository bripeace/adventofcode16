#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


void decrypt(char *letters, char *sid)
{
    int key = strtoul(sid, NULL, 0);
    char *c = letters;
    char *decrypted = calloc(strlen(letters)+1, sizeof *decrypted);
    char *d  = decrypted;
    while (*c) {
        if (*c != '-') {
            *d = (((*c -'a') + key) % 26)+'a';
        } else {
            *d = ' ';
        }
        d++;
        c++;
    }

    printf("decrypted: %s\t sid: %s\n", decrypted, sid);

    free(decrypted);
}

bool verify_checksum(int *letters, char *cs)
{
    char new_cs[6] = {0};
    int cur_max = 0;
    int max = 0;

    for (int i = 0; i < 5; i++) {
        for (int k = 0; k < 26; k++) {
           if (letters[k] > cur_max) {
                cur_max = letters[k];
                max = k;
           }
        }
        letters[max] = 0;
        new_cs[i] = max + 'a';
        cur_max = 0;
    }

    return !strcmp(new_cs, cs);
}

int main (int argc, char **argv)
{
    if (argc < 2) {
        printf("supply file on command line\n");
        return -1;
    }

    char *file_str = argv[1];
    printf ("File: %s\n",file_str);

    FILE *fp = fopen(file_str, "r");
    char line[80];
    while (fscanf(fp, "%s",&line[0]) == 1) {

        char *cs = &line[0];
        while (*cs != '[') {
            cs++;
        }

        char *sid = &cs[0];
        cs[0] = 0;
        cs++;
        cs[5] = 0;

        while (*sid != '-') {
            sid--;
        }
        sid[0] = 0;
        sid++;

        int lcount[26] = {0};
        char *i = &line[0];
        while (i != sid) {
            if (*i != '-') {
                lcount[*i -'a']++;
            }
            i++;
        }
        if (verify_checksum(lcount, cs)) {
            decrypt(line, sid);
        }
    }

}

