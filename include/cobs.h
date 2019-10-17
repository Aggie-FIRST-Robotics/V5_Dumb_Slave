/*
* Header for COBS encoding and decoding functions.
*
* @date 10/13/2019
* @author John Sauer, bakercp
*/

#pragma once

#include <cstdint>
#include <cstdlib>

namespace cobs
{
    size_t encode(const uint8_t* buffer,
                  size_t size,
                  uint8_t* encoded_buffer);

    size_t decode(const uint8_t* encoded_buffer,
                  size_t size,
                  uint8_t* decoded_buffer);

    size_t encoded_buffer_size(size_t unencoded_buffer_size);
};