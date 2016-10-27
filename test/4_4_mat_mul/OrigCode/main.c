
#include "stdio.h"
extern void mat_mul(int a[4][4],int b[4][4],int c[4][4]);

int main()
{
    int a[4][4]={{0,1,2,3},{4,5,6,7},{8,9,10,11},{12,13,14,15}};
    int b[4][4]={{16,17,18,19},{20,21,22,23},{24,25,26,27},{28,29,30,31}};
    int c[4][4];
    int i,j;

    mat_mul(a,b,c);
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
            printf("%d,",c[i][j]);
        printf("\n");
    }
    return 0;
}

