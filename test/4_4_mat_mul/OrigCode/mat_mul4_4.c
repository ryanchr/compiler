
void mat_mul(int a[4][4],int b[4][4],int c[4][4])
{
    int i;
    for(i=0;i<4;i++)
    {
        c[i][0]=a[i][0]*b[0][0]+a[i][1]*b[1][0]+a[i][2]*b[2][0]+a[i][3]*b[3][0];
        c[i][1]=a[i][0]*b[0][1]+a[i][1]*b[1][1]+a[i][2]*b[2][1]+a[i][3]*b[3][1];
        c[i][2]=a[i][0]*b[0][2]+a[i][1]*b[1][2]+a[i][2]*b[2][2]+a[i][3]*b[3][2];
        c[i][3]=a[i][0]*b[0][3]+a[i][1]*b[1][3]+a[i][2]*b[2][3]+a[i][3]*b[3][3];

    }
    return;
}
