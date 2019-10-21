/*
 * This computes the CRC-CCITT checksum for an array.
 * Taken from https://github.com/CANopenNode/CANopenNode/blob/master/stack/crc16-ccitt.c
 *
 * @date 10/13/2019
 * @author Janez Paternoster, John Sauer
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
uint16_t crc::crc16(uint8_t *data, size_t length)
{
    uint16_t crc = 0xFFFF;

    for(size_t i = 0; i < length; i++)
    {
        uint16_t tmp = (crc >> 8) ^ (uint16_t) data[i];
        crc = ((uint16_t)(crc << 8)) ^ crc::crc16_ccitt_table[tmp];
    }

    return crc;
}