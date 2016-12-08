#include <stdio.h>
#include <string.h>
#include <math.h>

typedef enum DIR {
   NORTH,
   SOUTH,
   EAST,
   WEST
} direction;

direction change_direction(direction current, char turn)
{
   switch (current)
   {
      case NORTH:
         if (turn == 'R')
         {
           return EAST; 
         }
         else
         {
            return WEST;
         }
         break;
      case SOUTH:
         if (turn == 'R')
         {
           return WEST; 
         }
         else
         {
            return EAST;
         }
         break;
      case EAST:
         if (turn == 'R')
         {
           return SOUTH; 
         }
         else
         {
            return NORTH;
         }
         break;
      case WEST:
         if (turn == 'R')
         {
           return NORTH; 
         }
         else
         {
            return SOUTH;
         }
         break;
   }

}

int main(void)
{
   char input[] = "R4, R5, L5, L5, L3, R2, R1, R1, L5, R5, R2, L1, L3, L4, R3, L1, L1, R2, R3, R3, R1, L3, L5, R3, R1, L1, R1, R2, L1, L4, L5, R4, R2, L192, R5, L2, R53, R1, L5, R73, R5, L5, R186, L3, L2, R1, R3, L3, L3, R1, L4, L2, R3, L5, R4, R3, R1, L1, R5, R2, R1, R1, R1, R3, R2, L1, R5, R1, L5, R2, L2, L4, R3, L1, R4, L5, R4, R3, L5, L3, R4, R2, L5, L5, R2, R3, R5, R4, R2, R1, L1, L5, L2, L3, L4, L5, L4, L5, L1, R3, R4, R5, R3, L5, L4, L3, L1, L4, R2, R5, R5, R4, L2, L4, R3, R1, L2, R5, L5, R1, R1, L1, L5, L5, L2, L1, R5, R2, L4, L1, R4, R3, L3, R1, R5, L1, L4, R2, L3, R5, R3, R1, L3";
   char hq[] = "R2,R2,R2";
   
   char *tok = strtok(input, ", ");

   direction current_dir = NORTH;
   int x =0, y = 0;
   while (tok)
   {
      char turn = tok[0];
      int len = strtoul(&tok[1],NULL, NULL);
      
      printf("Turn: %c\t Length: %d\n",turn, len);
      current_dir = change_direction(current_dir, turn);

      switch (current_dir)
      {
         case NORTH:
            y += len;
         break;
         case SOUTH:
            y -= len;
         break;
         case EAST:
            x += len; 
         break;
         case WEST:
            x -= len;
         break;
      }
     
      tok = strtok(NULL, ", ");
   }
   printf ("Final Location: %d %d\n",x, y);
   printf ("Distance      : %d\n", abs(0-x)+abs(0-y));
}


