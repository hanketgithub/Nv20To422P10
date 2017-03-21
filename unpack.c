/**
  * Convert P010 (packed 10 bit, 10 bit for 10 bit) to P016 (16 bit for 10 bit) 
  *
  */

#include <stdint.h>

#include "unpack.h"

/**
  *         LSB                                 MSB
  *  pix0   00  01  02  03  04  05  06  07   08  09
  *         <----------data[0]----------->  <data[1]>
  *
  *  pix0: [00...07] = y[0], [08, 09] = y[1] & 0x03;
  *
  *
  *  pix1   10  11  12  13  14  15   16  17  18  19
  *         <-------data[1]------>   <---data[2]--->
  *
  *  pix1: [10...17] = y[2], [18, 19] = y[3] & 0x03;
  *
  *
  *  pix2   20  21  22  23   24  25  26  27  28  29
  *         <---data[2]--->  <------data[3]------->
  *
  *  pix2: [20...27] = y[4], [28, 29] = y[5] & 0x03;
  *
  *
  *  pix3   30  31   32  33  34  35  36  37  38  39
  *        <data[3]> <----------data[4]------------>
  *
  *  pix3: [30...37] = y[6], [38, 39] = y[7] & 0x03;
  *
  *
  */
void unpack_y(uint8_t *y, quatre_pixel *q_pix)
{
    // pix0
    y[0] = q_pix->data[0];
    y[1] = q_pix->data[1] & 0x03;
    
    // pix 1
    y[2] = (q_pix->data[1] >> 2) | (q_pix->data[2] << 6);
    y[3] = (q_pix->data[2] & 0x0C) >> 2;
    
    // pix 2
    y[4] = (q_pix->data[2] >> 4) | (q_pix->data[3] << 4);
    y[5] = (q_pix->data[3] & 0x30) >> 4;
    
    // pix 3
    y[6] = (q_pix->data[3] >> 6) | (q_pix->data[4] << 2);
    y[7] = q_pix->data[4] >> 6;
}


/**
  *           LSB                                 MSB
  *  pix u0   00  01  02  03  04  05  06  07   08  09
  *           <----------data[0]----------->  <data[1]>
  *
  *  pix u0: [00...07] = u[0], [08, 09] = u[1] & 0x03;
  *
  *
  *  pix v0   10  11  12  13  14  15   16  17  18  19
  *           <-------data[1]------>   <---data[2]--->
  *
  *  pix v0: [10...17] = v[0], [18, 19] = v[0] & 0x03;
  *
  *
  *  pix u1   20  21  22  23   24  25  26  27  28  29
  *           <---data[2]--->  <------data[3]------->
  *
  *  pix u1: [20...27] = u[2], [28, 29] = u[3] & 0x03;
  *
  *
  *  pix v1   30  31   32  33  34  35  36  37  38  39
  *           <data[3]> <----------data[4]---------->
  *
  *  pix v1: [30...37] = v[2], [38, 39] = v[3] & 0x03;
  *
  *
  */
void unpack_uv(uint8_t *u, uint8_t *v, quatre_pixel *q_pix)
{
    // pix u0
    u[0] = q_pix->data[0];
    u[1] = q_pix->data[1] & 0x03;
    
    // pix u1
    u[2] = (q_pix->data[2] >> 4) | (q_pix->data[3] << 4);
    u[3] = (q_pix->data[3] & 0x30) >> 4;
    
    // pix v0
    v[0] = (q_pix->data[1] >> 2) | (q_pix->data[2] << 6);
    v[1] = (q_pix->data[2] & 0x0C) >> 2;
    
    // pix v1
    v[2] = (q_pix->data[3] >> 6) | (q_pix->data[4] << 2);
    v[3] = q_pix->data[4] >> 6;
}

