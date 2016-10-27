void mat_mul(int a[2][2],int b[2][2],int c[2][2],int x)
{
  int i,j;

  for(i=0;i<2;i++)
  {	
      c[i][0]=a[i][0]*b[0][0]+a[i][1]*b[1][0];
      c[i][1]=a[i][0]*b[0][1]+a[i][1]*b[1][1];
	}
  return;
}


