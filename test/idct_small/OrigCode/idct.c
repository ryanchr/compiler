
#define W1 28 /* 2048*sqrt(2)*cos(1*pi/16) */
#define W2 26 /* 2048*sqrt(2)*cos(2*pi/16) */
#define W3 24 /* 2048*sqrt(2)*cos(3*pi/16) */
#define W5 16 /* 2048*sqrt(2)*cos(5*pi/16) */
#define W6 13 /* 2048*sqrt(2)*cos(6*pi/16) */
#define W7 5  /* 2048*sqrt(2)*cos(7*pi/16) */

/* global declarations */
void init_idct (void);
void idct (short *block);

/* private data */
static short iclip[1024]; /* clipping table */
static  short *iclp;

/* private prototypes */
static void idctrow (short *blk);
static void idctcol (short *blk);

static void idctrow(short *blk)
{
  short x0, x1, x2, x3, x4, x5, x6, x7, x8;

  int k;
  /* shortcut */
  if (!((x1 = blk[4]<<11) | (x2 = blk[6]) | (x3 = blk[2]) |
        (x4 = blk[1]) | (x5 = blk[7]) | (x6 = blk[5]) | (x7 = blk[3])))
  {
    blk[0]=blk[1]=blk[2]=blk[3]=blk[4]=blk[5]=blk[6]=blk[7]=blk[0]<<3;
    return;
  }

  for ( k = 0; k<1; k++) {

  x0 = (blk[0]<<11) + 128; /* for proper rounding in the fourth stage */
  /* first stage */
  x8 = W7*(x4+x5);
  x4 = x8 + (W1-W7)*x4;
  x5 = x8 - (W1+W7)*x5;
  x8 = W3*(x6+x7);
  x6 = x8 - (W3-W5)*x6;
  x7 = x8 - (W3+W5)*x7;
  
  /* second stage */
  x8 = x0 + x1;
  x0 -= x1;
  x1 = W6*(x3+x2);
  x2 = x1 - (W2+W6)*x2;
  x3 = x1 + (W2-W6)*x3;
  x1 = x4 + x6;
  x4 -= x6;
  x6 = x5 + x7;
  x5 -= x7;
  
  /* third stage */
  x7 = x8 + x3;
  x8 -= x3;
  x3 = x0 + x2;
  x0 -= x2;
  x2 = (11*(x4+x5)+128)>>8;
  x4 = (11*(x4-x5)+128)>>8;

  /* fourth stage */
  blk[0] = (x7+x1)>>8;
  blk[1] = (x3+x2)>>8;
  blk[2] = (x0+x4)>>8;
  blk[3] = (x8+x6)>>8;
  blk[4] = (x8-x6)>>8;
  blk[5] = (x0-x4)>>8;
  blk[6] = (x3-x2)>>8;
  blk[7] = (x7-x1)>>8;
  }  
	return;
}
static void idctcol(short *blk)
{
  short x0, x1, x2, x3, x4, x5, x6, x7, x8;
  short addr0,addr1,addr2,addr3,addr4,addr5,addr6,addr7;

  int k;

  /* shortcut */
  if (!((x1 = (blk[8*4]<<8)) | (x2 = blk[8*6]) | (x3 = blk[8*2]) |
        (x4 = blk[8*1]) | (x5 = blk[8*7]) | (x6 = blk[8*5]) | (x7 = blk[8*3])))
  {
    blk[8*0]=blk[8*1]=blk[8*2]=blk[8*3]=blk[8*4]=blk[8*5]=blk[8*6]=blk[8*7]=
      iclp[(blk[8*0]+32)>>6];
    return;
  }
for ( k=0; k<1; k++) {

  x0 = (blk[8*0]<<8) + 8192;
  /* first stage */
  x8 = W7*(x4+x5) + 4;
  x4 = (x8+(W1-W7)*x4)>>3;
  x5 = (x8-(W1+W7)*x5)>>3;
  x8 = W3*(x6+x7) + 4;
  x6 = (x8-(W3-W5)*x6)>>3;
  x7 = (x8-(W3+W5)*x7)>>3;
  
  /* second stage */
  x8 = x0 + x1;
  x0 -= x1;
  x1 = W6*(x3+x2) + 4;
  x2 = (x1-(W2+W6)*x2)>>3;
  x3 = (x1+(W2-W6)*x3)>>3;
  x1 = x4 + x6;
  x4 -= x6;
  x6 = x5 + x7;
  x5 -= x7;
  
  /* third stage */
  x7 = x8 + x3;
  x8 -= x3;
  x3 = x0 + x2;
  x0 -= x2;
  x2 = (11*(x4+x5)+128)>>8;
  x4 = (11*(x4-x5)+128)>>8;
  addr0=(x7+x1)>>14;
  addr1=(x3+x2)>>14;
  addr2=(x0+x4)>>14;
  addr3=(x8+x6)>>14;
  addr4=(x8-x6)>>14;
  addr5=(x0-x4)>>14;
  addr6=(x3-x2)>>14;
  addr7=(x7-x1)>>14;

}
/* fourth stage */
  blk[8*0] = iclp[addr0];
  blk[8*1] = iclp[addr1];
  blk[8*2] = iclp[addr2];
  blk[8*3] = iclp[addr3];
  blk[8*4] = iclp[addr4];
  blk[8*5] = iclp[addr5];
  blk[8*6] = iclp[addr6];
  blk[8*7] = iclp[addr7];
return;
}

/* two dimensional inverse discrete cosine transform */
void idct(short *block)
{
  int i;

  for (i=0; i<8; i++)
    idctrow(block+8*i);

  for (i=0; i<8; i++)
    idctcol(block+i);
}

void init_idct()
{
  int i;

  iclp = iclip+512;
  for (i = -512; i<-256 ;i++ ) iclp[i] = -256;
  for (i = -256; i< 256 ;i++ ) iclp[i] = i;
  for (i =  256; i< 512 ;i++ ) iclp[i] = 255;
  /*for (i= -512; i<512; i++) {
    if ( i < -256 ) iclp[i] = -256;
    else if ( i > 255 ) iclp[i] = 255;
    else iclp[i] = i;
  }*/
}

int main()
{
  short block[64];
  int i,j;
  for(i=0;i<64;i++)
	block[i]=64-i;

  init_idct();
  idct(block);

  for(i=0;i<8;i++)
  {
   for(j=0;j<8;j++)
    printf("%d,",block[8*i+j]);
    printf("\n");
  }
  printf("\n"); 
  return 0;
}
