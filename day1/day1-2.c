#include <stdio.h>
#include <string.h>
#include <math.h>
#include <glib.h>
#include <malloc.h>

typedef enum DIR {
   NORTH,
   SOUTH,
   EAST,
   WEST
} direction;

typedef struct POINT {
   int x;
   int y;
} point;

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

int distance (point *a,point *b)
{
   return sqrt(pow((a->x - b->x),2) + pow((a->y - b->y),2));
}

gboolean is_on(point *a, point *b, point *c)
{
   return distance (a, c) + distance (c, b) == distance(a, b);
}

gboolean does_intercept(GList *list, point *p1)
{
   if (list == NULL)
      return FALSE;

   GList *i = list;
   
   point *p2 = i->data;
   i = i->next;

   gboolean ret = FALSE;
   while (i)
   {
      if (i->next == NULL)
         return FALSE;

      point *p3 = i->data;
      point *p4 = i->next->data;

      // Points are parallel
      if ( (p1->x-p2->x)*(p3->y-p4->y)-(p1->y-p2->y)*(p3->x-p4->x) == 0)
      {
         i = i->next;
         continue;
      }

      // find intersect
      point p = {
         ((p1->x * p2->y - p1->y * p2->x)*(p3->x - p4->x) - (p1->x - p2->x)*(p3->x*p4->y-p3->y*p4->x)) /
            ((p1->x-p2->x)*(p3->y-p4->y)-(p1->y-p2->y)*(p3->x-p4->x)),
         ((p1->x*p2->y-p1->y*p2->x)*(p3->y-p4->y) - (p1->y - p2->y)*(p3->x*p4->y-p3->y*p4->x)) /
            ((p1->x-p2->x)*(p3->y-p4->y)-(p1->y-p2->y)*(p3->x-p4->x))
      };

      //ensure intersect point is on both line segments
      if (is_on(p1, p2, &p) && is_on(p3, p4, &p))
      {
         printf("First Intersect point: (%2d, %2d) %d\n",p.x, p.y, (abs(p.x) + abs(p.y)));
         return TRUE;
      }

      i = i->next;

   }

   return ret;

}

int main(void)
{
   char input[] = "R4, R5, L5, L5, L3, R2, R1, R1, L5, R5, R2, L1, L3, L4, R3, L1, L1, R2, R3, R3, R1, L3, L5, R3, R1, L1, R1, R2, L1, L4, L5, R4, R2, L192, R5, L2, R53, R1, L5, R73, R5, L5, R186, L3, L2, R1, R3, L3, L3, R1, L4, L2, R3, L5, R4, R3, R1, L1, R5, R2, R1, R1, R1, R3, R2, L1, R5, R1, L5, R2, L2, L4, R3, L1, R4, L5, R4, R3, L5, L3, R4, R2, L5, L5, R2, R3, R5, R4, R2, R1, L1, L5, L2, L3, L4, L5, L4, L5, L1, R3, R4, R5, R3, L5, L4, L3, L1, L4, R2, R5, R5, R4, L2, L4, R3, R1, L2, R5, L5, R1, R1, L1, L5, L5, L2, L1, R5, R2, L4, L1, R4, R3, L3, R1, R5, L1, L4, R2, L3, R5, R3, R1, L3";

   char input_1[] = "R8, R4, R4, R8";
   
   char *tok = strtok(input, ", ");

   point p = {0};
   GList *point_list = g_list_prepend(NULL, &p);

   direction current_dir = NORTH;
   while (tok)
   {
      point *p = calloc(1, sizeof *p);
      *p = *(point *)point_list->data;
      char turn = tok[0];
      int len = strtoul(&tok[1],NULL, NULL);
      
      current_dir = change_direction(current_dir, turn);

      switch (current_dir)
      {
         case NORTH:
            p->y += len;
         break;
         case SOUTH:
            p->y -= len;
         break;
         case EAST:
            p->x += len; 
         break;
         case WEST:
            p->x -= len;
         break;
      }

      if (does_intercept(point_list, p))
         break;
           
      point_list = g_list_prepend(point_list, p);


      tok = strtok(NULL, ", ");
   }
/*   GList *i = point_list;

   while (i)
   {
      point *p = i->data;
      printf("(%2d,%2d)\n",p->x, p->y);
      i = i->next;
   }
  */ 
}


