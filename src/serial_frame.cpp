/*
 * Implementation of the serial_command and serial_frame_handler classes.
 *
 * @author John Sauer
 * @date 10/14/2019
 */
 
#include "serial_frame.h"

/*
 * Default constructor for serial_command.
 */
serial_command::serial_command() : 
  payload_size(0),
  address(0)
{
}

/*
 * Copy constructor for serial_command. This deep copies the command.
 */
serial_command::serial_command(const serial_command &old)
{
    /*
     * Copy fields from old packet.
     */
    payload_size = old.payload_size;
    address = old.address;

    /*
     * Copy each byte in the payload.
     */
    for(size_t i = 0; i < payload_size; i++)
    {
        data[i] = old.data[i];
    }
}

/*
 * Default destructor for serial_command.
 */
serial_command::~serial_command()
{
}

/*
 * Computes the checksum of a command.
 * Checksum is equal to the sum of every byte in the packet mod 256.
 *
 * @return The checksum of the command.
 */
uint8_t serial_command::checksum()
{
    /*
     * Add bytes from payload size and address.
     */
    uint32_t checksum = 0;
    checksum += payload_size;
    checksum += address >> 8;
    checksum += address & 0xff;

    /*
     * Add bytes from payload.
     */
    for(size_t i = 0; i < payload_size; i++)
    {
        checksum += data[i];
    }

    return static_cast<uint8_t>(checksum % 256);
}

/*
 * Returns if a command is a read request. Read requests have the Msb set.
 *
 * @return True if the command is a read request.
 */
bool serial_command::is_read()
{
    return (address & 0x8000) != 0;
}

/*
 * Parses a buffer containing a complete serial frame and copies every command 
 * to a queue. The format of a serial frame is:
 * 2 bytes: number of serial commands in frame
 * n bytes: serial commands
 * 2 bytes: CRC16 checksum of complete frame.
 * All fields are network order (big endian).
 *
 * @param buf The buffer containing the serial frame.
 * @param len The length of the buffer containing the serial frame.
 * @param queue The queue that parsed commands are placed into.
 *
 * @return True if parsing is successful.
 */
bool serial_frame_handler::buf2queue(uint8_t *buf,
                                     size_t len,
                                     abstract_queue<serial_command> &queue)
{
    /*
     * Immediately return if buffer is empty.
     */
    if(len == 0)
    {
        return false;
    }

    /*
     * The checksum is the last 2 bytes of the frame.
     */
    uint16_t checksum = (buf[len - 2] << 8) | buf[len - 1];
    uint16_t calc_crc = crc::crc16(buf, len-2);

    /*
     * If the CRC16 checksum in the frame does not match the computed value 
     * then return in failure.
     */
    if(calc_crc != checksum)
    {
        return false;
    }

    /*
     * The number of commands to parse is the first 2 bytes of the frame.
     */
    uint16_t command_num = (buf[0] << 8) | buf[1];

    /*
     * The starting index where serial commands are parsed from is offset by 2
     * because of the command number field at the beginning of the frame.
     */
    size_t command_index = 2;

    /*
     * Loop through every command indicated in the frame.
     */
    for(uint16_t i = 0; i < command_num; i++)
    {
        /*
         * Return in failure if the next 3 bytes (address + payload length)
         * exceed the input buffer capacity.
         */
        if(command_index + 3 >= len - 2)
        {
            return false;
        }

        /*
         * Create a new command object and set the payload length and
         * address from data in the frame.
         */
        serial_command command;
        command.payload_size = buf[command_index++];

        /*
         * Return in failure if indicated payload length is too long.
         */
        if(command.payload_size > 8)
        {
            return false;
        }

        command.address = (buf[command_index] << 8) | buf[command_index+1];
        command_index += 2;

        /*
         * Return in failure if the next bytes (payload + payload checksum + 
         * frame checksum) exceed the input buffer capacity.
         */
        if(command_index + command.payload_size + 3 > len)
        {
            return false;
        }

        /*
         * Copy payload data from frame to command object.
         */
        for(size_t j = 0; j < command.payload_size; j++)
        {
            command.data[j] = buf[command_index++];
        }

        /*
         * Return in failure if payload checksum in frame does not match
         * computed checksum.
         */
        if(buf[command_index++] != command.checksum())
        {
            return false;
        }

        /*
         * Add command to queue.
         */
        queue.push(command);
    }

    /*
     * Return true if all parsing completed without error.
     */
    return true;
}

/*
 * Creates a buffer with all of the commands in a queue. The format of a 
 * serial frame is:
 * 2 bytes: number of serial commands in frame
 * n bytes: serial commands
 * 2 bytes: CRC16 of complete frame.
 * All fields are network order (big endian).
 *
 * @param queue The queue commands are taken from.
 * @param buf The buffer that the frame is put into.
 * @param max_len The max length of the input frame.
 *
 * @return The length of the frame created or 0 if creation of frame unsuccessful.
 */
size_t serial_frame_handler::queue2buf(abstract_queue<serial_command> &queue,
                                       uint8_t *buf, size_t max_len)
{
    /*
     * Initialize the number of commands to 0 and the starting buffer length
     * to 2 since the first 2 bytes of the frame are for the number of commands.
     */
    uint16_t num_commands = 0;
    size_t buf_len = 2;

    /*
     * Keep looping until the queue is empty or there is not enough space 
     * left in the buffer. The required amount of space is at least 14 bytes
     * remaining in the buffer. The max size of a command is 12 bytes plus
     * 2 for the frame CRC plus 1 for the trailing 0.
     */
    while(!queue.empty() && buf_len + 15 < max_len)
    {
        /*
         * Get next command from queue and increment the command counter.
         */
        serial_command command = queue.pop();
        num_commands++;

        /*
         * Put payload size and address from command into buffer.
         */
        buf[buf_len++] = command.payload_size;
        buf[buf_len++] = static_cast<uint8_t>(command.address >> 8);
        buf[buf_len++] = static_cast<uint8_t>(command.address & 0xFF);

        /*
         * Put payload into buffer.
         */
        for(size_t j = 0; j < command.payload_size; j++)
        {
            buf[buf_len++] = command.data[j];
        }

        /*
         * Put checksum into buffer.
         */
        buf[buf_len++] = command.checksum();
    }

    /*
     * Set the first two bytes of packet to the number of commands processed.
     */
    buf[0] = static_cast<uint8_t>(num_commands >> 8);
    buf[1] = static_cast<uint8_t>(num_commands & 0xFF);

    /*
     * Put the frame CRC to the end of the buffer and return the buffer length.
     */
    uint16_t checksum = crc::crc16(buf, buf_len);
    buf[buf_len++] = static_cast<uint8_t>(checksum >> 8);
    buf[buf_len++] = static_cast<uint8_t>(checksum & 0xFF);

    buf[buf_len++] = 0;

    return buf_len;
}