/*
 *                                     16   12   5
 * this is the CCITT CRC 16 polynomial X  + X  + X  + 1.
 * This works out to be 0x1021, but the way the algorithm works
 * lets us use 0x8408 (the reverse of the bit pattern).  The high
 * bit is always assumed to be set, thus we only use 16 bits to
 * represent the 17 bit value.
 * Taken from https://people.cs.umu.se/isak/snippets/crc-16.c
 *
 * @date 10/13/2019
 * @author Isak Jonsson
 */

#include "crc16.h"

/*
 * Computes the CRC-CCITT checksum of an array.
 *
 * @param data_p The data being checksummed.
 * @param length The length of the data being checksummed.
 *
 * @return The CRC-CCITT checksum of the data.
 */
uint16_t crc::crc16(uint8_t *data_p, size_t length)
{
    uint8_t i;
    uint32_t data;
    uint32_t crc = 0xffff;

    /*
     * Return immediately if data is empty.
     */
    if (length == 0)
    {
        return (~crc);
    }

    /*
     * Loop through every element in the array.
     */
    do
    {
        /*
         * Loop though every bit in each byte.
         */
        for (i=0, data=(uint32_t)0xff & *data_p++;
             i < 8; 
             i++, data >>= 1)
        {
            if ((crc & 0x0001) ^ (data & 0x0001))
            {
                crc = (crc >> 1) ^ POLY;
            }
            else
            {
                crc >>= 1;
            }
        }
    } while (--length);

    crc = ~crc;
    data = crc;
    crc = (crc << 8) | (data >> 8 & 0xff);

    return (crc);
}