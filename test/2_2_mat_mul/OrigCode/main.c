#include "stdio.h"
extern void mat_mul(int a[2][2],int b[2][2],int c[2][2]);


int main()
{
    int a[2][2]={{1,2},{4,5}};
    int b[2][2]={{6,7},{8,9}};
    int c[2][2];
    int i,j;

    mat_mul(a,b,c);
    for(i=0;i<2;i++)
    {
				for(j=0;j<2;j++)
					printf("%d,",c[i][j]);
    	  printf("\n");
    }
    
    return 0;
}

