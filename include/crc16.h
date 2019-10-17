/*
 * Header for CRC16 functions.
 *
 * @date 10/13/2019
 * @author John Sauer, Isak Jonsson
 */

#pragma once

#include <cstdint>
#include <cstdlib>

namespace crc
{

    /*
     * The CR-CCITT polynomial
     */
    constexpr uint32_t POLY = 0x8408;

    uint16_t crc16(uint8_t *data_p, size_t length);
}