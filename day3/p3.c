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

    int a[3], b[3], c[3];
    int valid = 0;
    int cont = 1;
    while (cont)
    {
        for (int i = 0; i < 3; i++)
        {
            cont = cont && fscanf(fp, "  %d  %d  %d",&a[i],&b[i],&c[i]) == 3;
        }
        if (!cont)
            break;

        if (valid_tri(a[0],a[1],a[2]))
        {
            valid++;
            printf("valid %4d: %d %d %d\n",valid,a[0],a[1],a[2]);
                    }
        if (valid_tri(b[0],b[1],b[2]))
        {
            valid++;
            printf("valid %4d: %d %d %d\n",valid,b[0],b[1],b[2]);
        }
        if (valid_tri(c[0],c[1],c[2]))
        {
            valid++;
            printf("valid %4d: %d %d %d\n",valid,c[0],c[1],c[2]);
        }


    }
    printf ("%d valid triangles\n", valid);
            
}
