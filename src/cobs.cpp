/*
* COBS encoding and decoding functions.
* Shamelessly lifted from
* https://github.com/bakercp/PacketSerial/blob/master/src/Encoding/COBS.h
*
* @date 10/13/2019
* @author John Sauer, bakercp
*/

#include "cobs.h"

/*
 * Encodes a message using COBS encoding.
 * 
 * @param buffer The buffer containing the message being encoded.
 * @param size The length of the message being encoded.
 * @param encoded_buffer The buffer containing the encoded message.
 *
 * @return The size of the encoded message.
 */
size_t cobs::encode(const uint8_t* buffer,
              size_t size,
              uint8_t* encoded_buffer)
{
    /*
     * Set initial indices and values.
     */
    size_t read_index = 0;
    size_t write_index = 1;
    size_t code_index = 0;
    uint8_t code = 1;

    /*
     * Loop until entire source buffer is consumed.
     */
    while (read_index < size)
    {
        /*
         * Update code index if the current value in the buffer equals 0.
         */
        if (buffer[read_index] == 0)
        {
            encoded_buffer[code_index] = code;
            code = 1;
            code_index = write_index++;
            read_index++;
        }
        else
        {
            /*
             * Place current value in buffer into encoded buffer.
             */
            encoded_buffer[write_index++] = buffer[read_index++];
            code++;

            /*
             * Update code index if code equals 255.
             */
            if (code == 0xFF)
            {
                encoded_buffer[code_index] = code;
                code = 1;
                code_index = write_index++;
            }
        }
    }

    /*
     * Final update to code index.
     */
    encoded_buffer[code_index] = code;

    return write_index;
}

/*
 * Decodes a message using COBS encoding. This implementation removes
 * leading zeroes and stops at the first instance of a zero after the 
 * leading zeroes.
 * 
 * @param encoded_buffer The buffer containing the encoded message.
 * @param size The length of the encoded message.
 * @param decoded_buffer The buffer containing the decoded message.
 *
 * @return The size of the decoded message or 0 if message could not
 * be decoded.
 */
size_t cobs::decode(const uint8_t* encoded_buffer,
              size_t size,
              uint8_t* decoded_buffer)
{
    /*
     * Return failure if input buffer is empty.
     */
    if (size == 0)
         return 0;

    /*
     * Set initial indices and values.
     */
    size_t read_index = 0;
    size_t write_index = 0;
    uint8_t code = 0;
    uint8_t i = 0;

    /*
     * Loop through buffer until a nonzero value is found
     * to remove leading zeroes.
     */
    while (encoded_buffer[read_index] == 0)
    {
        /*
         * Return failure if the entire input buffer is consumed.
         */
        if (++read_index == size)
        {
          return 0;
        }
    }

    /*
     * Loop until entire input buffer is consumed.
     */      
    while (read_index < size)
    {
        /*
         * Get code from current position in source buffer.
         */
        code = encoded_buffer[read_index];

        /*
         * Stop if code equals 0.
         */
        if (code == 0)
        {
            return write_index;
        }

        /*
         * Return failure if next code is out of bounds.
         */
        if (read_index + code > size && code != 1)
        {
            return 0;
        }

        read_index++;

        /*
         * Loop through values until next code.
         */
        for (i = 1; i < code; i++)
        {
            /*
             * Place current value into decoded buffer unless it is 0,
             * then return in failure.
             */
            if(encoded_buffer[read_index] != 0)
            {
                decoded_buffer[write_index++] = encoded_buffer[read_index++];
            }
            else
            {
                return 0;
            }
        }
    }

    return write_index;
}

/*
 * Return the encoded size of a buffer.
 *
 * @param unencoded_buffer_size The unencoded buffer size.
 *
 * @return The encoded buffer size.
 */
size_t cobs::encoded_buffer_size(size_t unencoded_buffer_size)
{
    return unencoded_buffer_size + unencoded_buffer_size / 254 + 1;
}