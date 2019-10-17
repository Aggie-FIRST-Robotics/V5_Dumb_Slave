/*
 * Contains data structures and functions for frame parsing and creation.
 *
 * @author John Sauer
 * @date 10/14/2019
 */
#pragma once

#include <cstdlib>
#include <cstdint>
#include "abstract_queue.h"
#include "crc16.h"

/*
 * This is the struct representing a single serial command.
 * The network format is:
 * 1 byte: payload size
 * 2 bytes: address
 * 0-8 bytes: payload
 * 1 byte: checksum
 * All values are network order (big endian).
 */
struct serial_command
{
    /*
     * The maximum allowable size for the payload.
     */
    static constexpr size_t MAX_COMMAND_LEN = 8;

    /*
     * The length of the payload for this command.
     */
    uint8_t payload_size;

    /*
     * The address of this command.
     */
    uint16_t address;

    /*
     * The data contained in the payload.
     */
    uint8_t data[MAX_COMMAND_LEN];

    /*
     * Member functions.
     */
    serial_command();
    ~serial_command();
    serial_command(const serial_command &old);
    uint8_t checksum();
    bool is_read();
};

/*
 * Namespace contained frame creating and parsing functions.
 */
namespace serial_frame_handler
{
    bool buf2queue(uint8_t *buf, 
                          size_t len,
                          abstract_queue<serial_command> &queue);

    size_t queue2buf(abstract_queue<serial_command> &queue,
                            uint8_t *buf, size_t max_len);

}
