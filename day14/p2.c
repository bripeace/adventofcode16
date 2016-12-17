#include <openssl/md5.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include <aoc.h>

#define TO_HEX(x) (x <= 9 ? '0' + x : 'a' + x - 10)
#define TO_INT(x) (x <= '9' ? x - '0' : x - 'a' + 10)

static int keys_found;
char *Hash(const char *str)
{
   unsigned char digest[16];
   MD5_CTX ctx;
   MD5_Init(&ctx);
   MD5_Update(&ctx, str, strlen(str));
   MD5_Final(digest, &ctx);

   char *hash = calloc(33, sizeof *hash);
   for (int i = 0; i < 16; ++i)
      sprintf(&hash[i*2], "%02x", (unsigned int)digest[i]);
   return hash;
}

char *SHash(const char *str)  
{
    char *hash = Hash(str);
    for (int j  = 0; j < 2016; j++) {
        char *new_hash = Hash(hash);
        free(hash);
        hash = new_hash;
    }

    return hash;

}

#define CACHE_SIZE 50000
char *cache_shash(const char *salt, unsigned long int i) 
{
    static char **cache = NULL;

    if (i >= CACHE_SIZE) 
        return NULL;

    if (salt == NULL) {
        for(int a = 0; a < CACHE_SIZE; a++) {
           free(cache[a]);
        } 
        free(cache);
        return NULL;
    }

    if (cache == NULL) {
        cache = calloc(CACHE_SIZE, sizeof *cache);
    }

    if (cache[i] != NULL) {
        return cache[i];
    } else {
        char *str = calloc(strlen(salt) + 12 + 1, sizeof *str);
        sprintf(str, "%s%lu", salt, i);
        char *hash = SHash(str);
        free(str);
        cache[i] = hash;
        return hash;
    }
}


int main(void)
{
    char *salt = "ihaygndm";

    unsigned long int i = 0;
    char *hash = NULL;
    for (;;) {
new_hash:

        hash = cache_shash(salt, i);
        char *h = hash;

        while (*h && 32-(h-hash) > 2) {
            if (keys_found >= 4) {
                printf("%lu\n",--i);
                goto end;
            }

            if (h[0] == h[1] && h[0] == h[2]) {
                for (int a = 1; a < 1000; a++) {
                    char *hash2 = cache_shash(salt, a+i);
                    char *q = hash2;
                    while (*q && 32-(q-hash2) > 4) {
                        if (q[0] == h[0] && q[0] == q[1] && q[1] == q[2] && q[2] == q[3] &&
                                q[3] == q[4])  {
                            keys_found++;
                            printf("%5lu - %5lu - %c - %.7s - %s\n",i, a+i, h[0], q, h);
                            i++;
                            goto new_hash;
                        }
                        q++;
                    }
                }
                i++;
                goto new_hash;
            }
            h++;
        }
        i++;
    }

end:
    cache_shash(NULL, 0); 
}
