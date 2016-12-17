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

int main(void)
{
   char *salt = "ihaygndm";

   size_t s = strlen(salt) + 1 + 10;
   unsigned long int i = 0;
   char *str = calloc(s, sizeof *str);
   char *hash = NULL;
   for (;;) {
new_hash:
      snprintf(str, s, "%s%lu",salt, i);
       
      hash = Hash(str);
      char *h = hash;

      while (*h && 32-(h-hash) > 2) {
          if (keys_found >= 64) {
              printf("%lu\n",--i);
              goto end;
          }

          if (h[0] == h[1] && h[0] == h[2]) {
              for (int a = 1; a < 1000; a++) {
                  char str2[100];
                  sprintf(str2,"%s%lu",salt, a+i);
                  char *hash2 = Hash(str2);
                  char *q = hash2;
                  while (*q && 32-(q-hash2) > 4) {
                      if (q[0] == h[0] && q[0] == q[1] && q[1] == q[2] && q[2] == q[3] &&
                              q[3] == q[4])  {
                          keys_found++;
                          printf("%5lu - %5lu - %c - %.7s - %s\n",i, a+i, h[0], q, h);
                          free(hash2);
                          i++;
                          free(hash);
                          goto new_hash;
                      }
                      q++;
                  }
                  free(hash2);
              }
              i++;
              free(hash);
              goto new_hash;
          }
          h++;
      }
      free(hash);
      i++;
   }

end:
free(hash);
free(str);

}
