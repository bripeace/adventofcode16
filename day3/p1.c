#include <stdio.h>
int valid_tri(int a, int b, int c)
{
    return ((a + b) > c && (a + c) > b && (b + c) > a);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf ("supply file name on command line\n");
        return -1;
    }

    char *file = argv[1];
    FILE *fp = fopen(file, "r"); 

    int a, b, c;
    int valid = 0;
    while (fscanf(fp, "  %d  %d  %d",&a,&b,&c) == 3)
    {
        if (valid_tri(a,b,c))
        {
            printf("%3d %3d %3d\n",a,b,c);
            valid++;
        }
    }
    printf ("%d valid triangles\n", valid);
            
}
