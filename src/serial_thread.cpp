/*
 * Implementation of serial_thread class.
 *
 * @author John Sauer
 * @date 10/14/2019
 */

#include "serial_thread.h"

/*
 * This function initializizes and spawns a new thread containing 
 * the serial I/O routine for a smart port.
 *
 * @param brain The VEX Brain object.
 * @param port The smart port that this thread is operating upon. The range of
 * allowable values is 0 (Port 1) to 20 (Port 21).
 * @param baudrate The baudrate the serial port should communicate at.
 * @param callback A callback function that is used for thread creation.
 * This function must contain a call to serial_routine for this object to
 * function correctly.
 */
void serial_thread::init(vex::brain &brain, 
                         int32_t port,
                         int32_t baudrate,
                         int(*callback)(void))
{   
    brain_ptr = &brain;
    port_ = port;
    baudrate_ = baudrate;

    /*
     * Create serial communication thread and detach it.
     */
    ser_thread = vex::task(callback);
}

/*
 * This function signals the serial routing to end.
 */
void serial_thread::destroy()
{
    terminated.set_value(true);
}

/*
 * Returns the number of successfully received frames.
 *
 * @return The number of successfully received frames.
 */
size_t serial_thread::rx_frames()
{
    return rx_frames_.get_value();
}

/*
 * Returns the number of successfully transmitted frames.
 *
 * @return The number of successfully transmitted frames.
 */
size_t serial_thread::tx_frames()
{
    return tx_frames_.get_value();
}

/*
 * Returns the number of receive errors.
 *
 * @return The number of receive errors.
 */
size_t serial_thread::rx_errors()
{
    return rx_errors_.get_value();
}

/*
 * Returns the number of transmit errors.
 *
 * @return The number of transmit errors.
 */
size_t serial_thread::tx_errors()
{
    return tx_errors_.get_value();
}

/*
 * Returns the serial command receive queue.
 *
 * @return The serial command receive queue.
 */
atomic_ringbuffer<serial_command, serial_thread::QUEUE_SIZE>
&serial_thread::rx_queue()
{
    return rx_queue_;
}

/*
 * Returns the serial command transmit queue.
 *
 * @return The serial command transmit queue.
 */
atomic_ringbuffer<serial_command, serial_thread::QUEUE_SIZE>
&serial_thread::tx_queue()
{
    return tx_queue_;
}

/*
 * Function that runs in separate thread that handles serial I/O
 * for a single smart port.
 */
void serial_thread::serial_routine()
{
    /*
     * Initialize counters and state machine.
     */
    terminated.set_value(false);
    rx_frames_.set_value(0);
    tx_frames_.set_value(0);
    rx_errors_.set_value(0);
    tx_errors_.set_value(0);
    ser_state = START_RECEIVE;

    /*
     * Configure smart port.
     */
    smart_port = vexDeviceGetByIndex(port_);
    vexDeviceGenericSerialEnable(smart_port, 0);
    vexDeviceGenericSerialBaudrate(smart_port, baudrate_);

    while(!terminated.get_value())
    {
        /*
         * Each iteration should take ITER_TIME ms.
         */
        uint32_t iteration_time = brain_ptr->Timer.system() + ITER_TIME;

        /*
         * Run serial communication state machine.
         */
        switch(ser_state)
        {
            case TRANSMITTING:
            {
                /*
                 * Get serial commands from transmit queue.
                 */
                size_t decoded_buf_len = 
                  serial_frame_handler::queue2buf(tx_queue_,
                                                  decoded_buffer,
                                                  DECODED_BUF_CAP);

                /*
                 * Report error and reset if queue2buf fails.
                 */
                if(decoded_buf_len == 0)
                {
                    tx_errors_.set_value(tx_errors_.get_value() + 1);
                    ser_state = START_RECEIVE;
                    break;
                }

                /*
                 * COBS encode frame.
                 */
                size_t tx_len = cobs::encode(decoded_buffer,
                                             decoded_buf_len,
                                             tx_buf);

                if(tx_len > 0)
                {
                    /*
                     * Send encoded serial frame.
                     */
                    if(vexDeviceGenericSerialTransmit(smart_port, 
                                                      tx_buf,
                                                      tx_len) 
                       == tx_len)
                    {
                        tx_frames_.set_value(tx_frames_.get_value() + 1);
                    }
                    /*
                     * If transmit fails by not sending the number of bytes
                     * expected, then report an error.
                     */
                    else
                    {
                        tx_errors_.set_value(tx_errors_.get_value() + 1);
                    }
                }
                /*
                 * If COBS encoding fails, then report an error.
                 */
                else
                {
                    tx_errors_.set_value(tx_errors_.get_value() + 1);
                }
                
                /*
                 * Always reset after this state since this set of operations
                 * should complete in one iteration.
                 */
                ser_state = START_RECEIVE;
                break;
            }
            case START_RECEIVE:
            {
                rx_buf_len = 0;

                int32_t read_char;
                while((read_char = vexDeviceGenericSerialReadChar(smart_port)) >= 0)
                {

                    /*
                     * Skip all leading zeroes.
                     */
                    if(read_char != 0)
                    {
                        rx_buf[rx_buf_len++] = static_cast<uint8_t>(read_char);
                        rx_timeout = brain_ptr->Timer.system() + TIMEOUT;
                        ser_state = RECEIVING;
                        break;
                    }
                }

                break;
            }
            case RECEIVING:
            {
                /*
                 * Report error and reset if current time exceeds timeout.
                 */
                if(brain_ptr->Timer.system() > rx_timeout)
                {
                    rx_errors_.set_value(rx_errors_.get_value()+1);
                    ser_state = START_RECEIVE;
                }

                int32_t read_char;
                while((read_char = vexDeviceGenericSerialReadChar(smart_port)) >= 0)
                {

                    /*
                     * Stop if read character is 0, decode COBS encoded receive frame,
                     * enqueue serial commands, and update state.
                     */
                    if(read_char == 0)
                    {
                        size_t decoded_buf_len = cobs::decode(rx_buf,
                                                              rx_buf_len,
                                                              decoded_buffer);

                        if(decoded_buf_len == 0)
                        {
                            rx_errors_.set_value(rx_errors_.get_value()+1);
                            ser_state = START_RECEIVE;
                            break;
                        }

                        /*
                         * Update frames received and go to tramsmit if queueing successful.
                         */
                        if(serial_frame_handler::buf2queue(decoded_buffer,
                                                           decoded_buf_len,
                                                           rx_queue_))
                        {
                            rx_frames_.set_value(rx_frames_.get_value()+1);
                            ser_state = TRANSMITTING;
                        }

                        /*
                         * Report error and reset if queueing failed.
                         */
                        else
                        {
                            rx_errors_.set_value(rx_errors_.get_value()+1);
                            ser_state = START_RECEIVE;
                        }
                        
                        break;
                    }

                    /*
                     * If character is nonzero and valid, add it to receive frame buffer.
                     */
                    else
                    {
                        rx_buf[rx_buf_len++] = static_cast<uint8_t>(read_char);
                        
                        /*
                         * Report error and reset if receive frame buffer grows out of bounds.
                         */
                        if(rx_buf_len == RX_BUF_CAP)
                        {
                            rx_errors_.set_value(rx_errors_.get_value()+1);
                            ser_state = START_RECEIVE;
                            break;
                        }
                    }
                }

                break;
            }
        }

        /*
         * Pause until ITER_TIME ms have elapsed since the beginning 
         * of the iteration.
         */
        vex::this_thread::sleep_until(iteration_time);
    }
}