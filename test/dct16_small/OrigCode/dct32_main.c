
#include <stdio.h>
#include "dct32.h"
int main()
{
	int i;
	mad_fixed_t in[32] = { 
		2341, 2, 3457, 423, 5, 6, 22456, 8,
		9456, 2, 14, 32, 4, 4, 5, 6,
		223, 324, 7, 4, 18,12256,3, 3,
		4, 9235, 0, 8352, 1, 11,3, 2325
	};
	mad_fixed_t lo[16][8], hi[16][8];
	dct32(in,0,lo,hi);
	printf("low 16 :\n");
	for ( i = 0; i<16;i++)
	{
		printf("%d\n",lo[i][0]);
	}
	printf("high 16 :\n");
	for ( i = 0; i<16;i++)
	{
		printf("%d\n",hi[i][0]);
	}
		                      
}
