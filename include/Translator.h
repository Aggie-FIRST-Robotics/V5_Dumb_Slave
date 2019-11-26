//
// Created by aamhi on 11/25/2019.
//

#ifndef V5_DUMB_SLAVE_TRANSLATOR_H
#define V5_DUMB_SLAVE_TRANSLATOR_H

#include <stdint-gcc.h>
#include <vector>

class Translator{
std::vector<int8_t> table;

/**
 * Sets the system corresponding to the address to the payload data.
 *
 * @param address The address being written to.
 * @param payload The array containing the payload. Data is big endian.
 * @param payload_len The length of the payload.
 * @return True if successful, false if not.
 */
    bool write_command(uint16_t address, uint8_t *payload, uint8_t payload_len)
    {
        table.


    }

/**
 * Gets the payload data corresponding to a particular address.
 *
 * @param address The address being read from.
 * @param payload The array containing the payload. Data is big endian.
 * @param payload_len The output length of the payload.
 * @return True if successful, false if not.
 */
    bool read_command(uint16_t address, uint8_t *payload, uint8_t &payload_len)
    {
        return 1;


    }
};
#endif //V5_DUMB_SLAVE_TRANSLATOR_H
