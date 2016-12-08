#include <openssl/md5.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

char *Hash(char *str)
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

bool first_five_are_zero(char *s)
{
   int count = 0;
   while (*s) {
      if (*s != '0' || count == 5) {
         break;
      }
      s++;
      count++;
   }
   return count == 5;
}

void find_password(char *id)
{
   
   size_t s = strlen(id) + 1 + 20;
   char *hashstr = calloc(s, sizeof *hashstr);
   int found = 0; int index = 0;
   char password[9] = {0};
   while (found != 8)
   {
      snprintf(hashstr, s, "%s%d", id, index);
     // printf("searching %s\n",hashstr);
      char *hash = Hash(hashstr);
      if (first_five_are_zero(hash))
      {
         printf("str: %s hash: %s %d letter: %c\n",hashstr,hash,found+1, hash[5]);
         password[found] = hash[5];
         found++;
      }
      free(hash);
      index++;

   }
   printf("Password: %s\n",password);


}

int main(int argc, char **argv)
{
   char *hello = "cxdnnyjw";

   find_password(hello);

}
