/*
* COBS encoding and decoding functions.
* The encode function was shamelessly lifted from
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

    /*
     * Add trailing zero to mark the end of message.
     */ 
    encoded_buffer[write_index++] = 0;

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
     * Return immediately if the input buffer is empty.
     */

    if (size == 0)
    {
        return 0;
    }

    /*
     * Initialize state variables and pointers
     */
    bool is_zero = false;
    size_t read_index = 0;
    size_t write_index = 0;

    while(read_index < size)
    {
        uint8_t code_count = encoded_buffer[read_index++];

        /*
         * The frame is complete if the count is 0.
         */
        if(code_count == 0)
        {
            return write_index;
        }

        /*
         * Add a zero to the output if this iteration should add a 0.
         */
        if(is_zero)
        {
            decoded_buffer[write_index++] = 0;
        }

        /*
         * Return in failure if the input buffer is smaller than indicated
         * by the code count.
         */
        if(size < read_index + code_count - 1)
        {
            return 0;
        }

        /*
         * Loop through the code count - 1. The last element is the next 
         * code count.
         */
        for(uint8_t count = 0; count < code_count - 1; count++)
        {
            uint8_t read_val = encoded_buffer[read_index++];

            /*
             * If the read value at this point is 0, this is not a valid 
             * COBS frame.
             */
            if(read_val == 0)
            {
                return 0;
            }

            decoded_buffer[write_index++] = read_val;
        }

        /*
         * If the code count is 255, then there is not a zero following 
         * the set of nonzero reads.
         */
        is_zero = code_count != 0xFF;
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