
/**
 * @file bits.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Sat Nov  2 18:48:44 2013
 * @brief Bit twiddling.
 *
 * Bit twiddling.
 */

#ifndef _BITS_H_
#define _BITS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

unsigned int get_host_endianness( void );
byte *__flip_endianness( byte * block, uint sz );

  /**
   * @brief Flip endianness, @see __flip_endianness . 
   * @param x Bunch of bytes to be flipped. 
   */
#define FLIP_ENDIANNESS( x ) __flip_endianness( (byte*)&(x), sizeof(x) )

  /**
   * @brief Clear bit in byte. 
   * @param b Byte. 
   * @param n Bit number to clear. 
   */
#define CLR_BIT( b, n ) ( b &= 0xFF-(1<<(n)) )

  /**
   * @brief Set bit in byte. 
   * @param b Byte. 
   * @param n Bit number to set. 
   */
#define SET_BIT( b, n ) ( b |= 1<<(n) )

  /**
   * @brief Read bit in byte. 
   * @param b Byte. 
   * @param n Bit number to read. 
   */
#define GET_BIT( b, n ) ( ( b & 1<<(n) ) >> (n) )

  /**
   * @brief Read bits in byte. 
   * @param b Byte. 
   * @param i First bit number to read, inclusive. 
   * @param j Last bit number to read, inclusive. 
   */
#define GET_BITS( b, i, j ) ( b & (((1 << (j-i)) - 1) << i ) )


#ifdef __cplusplus
}
#endif

#endif /* _BITS_H_ */
