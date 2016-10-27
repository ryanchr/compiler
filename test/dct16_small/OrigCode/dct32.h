#ifndef DCT32_H
#define DCT32_H

#  define mad_fixed_t short
#  define MAD_F(x) (x)
#  define MUL(x, y) ((((x) + (1L << 5)) >> 6) *  (((y) + (1L << 7)) >> 8))

#  define SHIFT(x)  (((x) + (1L << 5)) >> 6) 

#  define costab1	MAD_F(0x0ffb)  /* 0.998795456 */
#  define costab2	MAD_F(0x0fec)  /* 0.995184727 */
#  define costab3	MAD_F(0x0fd3)  /* 0.989176510 */
#  define costab4	MAD_F(0x0fb1)  /* 0.980785280 */
#  define costab5	MAD_F(0x0f85)  /* 0.970031253 */
#  define costab6	MAD_F(0x0f4f)  /* 0.956940336 */
#  define costab7	MAD_F(0x0f10)  /* 0.941544065 */
#  define costab8	MAD_F(0x0ec8)  /* 0.923879533 */
#  define costab9	MAD_F(0x0e76)  /* 0.903989293 */
#  define costab10	MAD_F(0x0e1c)  /* 0.881921264 */
#  define costab11	MAD_F(0x0db9)  /* 0.857728610 */
#  define costab12	MAD_F(0x0d4d)  /* 0.831469612 */
#  define costab13	MAD_F(0x0cd9)  /* 0.803207531 */
#  define costab14	MAD_F(0x0c5e)  /* 0.773010453 */
#  define costab15	MAD_F(0x0bda)  /* 0.740951125 */
#  define costab16	MAD_F(0x0b50)  /* 0.707106781 */
#  define costab17	MAD_F(0x0abe)  /* 0.671558955 */
#  define costab18	MAD_F(0x0a26)  /* 0.634393284 */
#  define costab19	MAD_F(0x0987)  /* 0.595699304 */
#  define costab20	MAD_F(0x08e3)  /* 0.555570233 */
#  define costab21	MAD_F(0x0839)  /* 0.514102744 */
#  define costab22	MAD_F(0x078a)  /* 0.471396737 */
#  define costab23	MAD_F(0x06d7)  /* 0.427555093 */
#  define costab24	MAD_F(0x061f)  /* 0.382683432 */
#  define costab25	MAD_F(0x0563)  /* 0.336889853 */
#  define costab26	MAD_F(0x04a5)  /* 0.290284677 */
#  define costab27	MAD_F(0x03e3)  /* 0.242980180 */
#  define costab28	MAD_F(0x031f)  /* 0.195090322 */
#  define costab29	MAD_F(0x0259)  /* 0.146730474 */
#  define costab30	MAD_F(0x0191)  /* 0.098017140 */
#  define costab31	MAD_F(0x00c8)  /* 0.049067674 */

void dct32(mad_fixed_t in[32], unsigned int slot,
	   mad_fixed_t lo[16][8], mad_fixed_t hi[16][8]);

#endif 
