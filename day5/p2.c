#include <openssl/md5.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

void Hash(char *str, char *hash)
{
   unsigned char digest[16];
   MD5_CTX ctx;
   MD5_Init(&ctx);
   MD5_Update(&ctx, str, strlen(str));
   MD5_Final(digest, &ctx);

   for (int i = 0; i < 16; ++i)
      sprintf(&hash[i*2], "%02x", (unsigned int)digest[i]);
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
   
   size_t s = strlen(id) + 10 + 1;
   char *hashstr = calloc(s, sizeof *hashstr);
   int found = 0; int index = 0;
   char password[9] = "________";
   char foundpos[9] = {0};
   while (found != 8)
   {
      snprintf(hashstr, s, "%s%d", id, index);
      char hash[33];
      Hash(hashstr, hash);
      if (first_five_are_zero(hash))
      {
         int pos = hash[5] - '0';
         if (pos < 8)
         {
            char letter = hash[6];
            if (!foundpos[pos])
            {
               foundpos[pos] = 1;
               password[pos] = letter;
               found++;
               printf("str: %-17s hash: %s password: %s\n",hashstr,hash,password);
            }
         }
      }
      index++;

   }
   printf("Password: %s\n",password);
   free(hashstr);
}

int main(int argc, char **argv)
{
   char *hello = "cxdnnyjw";

   find_password(hello);

}
