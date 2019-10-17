/*
 * This header contains the classes that manage serial I/O threads.
 *
 * @author John Sauer
 * @date 10/14/2019
 */

#pragma once

#include "vex.h"
#include "atomic_primitive.h"
#include "atomic_ringbuffer.h"
#include "cobs.h"
#include "serial_frame.h"


/*
 * Enum for serial I/O state machine.
 */
enum serial_state
{
    TRANSMITTING,
    START_RECEIVE,
    RECEIVING
};

/*
 * This class creates and maintains a new thread that handles serial I/O
 * for a single smart port.
 */
class serial_thread
{
    /*
     * The capacity of the receive frame buffer.
     */
    static constexpr size_t RX_BUF_CAP = 4096;

    /*
     * The capacity of the transmit frame buffer.
     */
    static constexpr size_t TX_BUF_CAP = 4096;

    /*
     * The capacity of the decoded frame buffer.
     */
    static constexpr size_t DECODED_BUF_CAP = 4080;

    /*
     * The capacity of the transmit and receive serial command queues.
     */
    static constexpr size_t QUEUE_SIZE = 1024;

    /*
     * The receive timeout period, in milliseconds.
     */
    static constexpr uint32_t TIMEOUT = 10;

    /*
     * The period of each serial routine period, in milliseconds.
     */
    static constexpr uint32_t ITER_TIME = 2;

    public:

    /*
     * Running in main thread.
     */
    void init(const vex::brain &brain,
              int32_t port,
              int32_t baudrate,
              void(*callback)(void));
    
    void destroy();
    size_t rx_frames();
    size_t rx_errors();
    size_t tx_frames();
    size_t tx_errors();
    atomic_ringbuffer<serial_command, QUEUE_SIZE> &rx_queue();
    atomic_ringbuffer<serial_command, QUEUE_SIZE> &tx_queue();

    /*
     * Running in ser_thread.
     */
    void serial_routine();

    private:

    /*
     * Main thread fields.
     */

    /*
     * The thread that the serial routine is running on.
     */
    vex::thread ser_thread;

    /*
     * Synchronized fields.
     */

    /*
     * If the serial routine should terminate next iteration.
     */
    atomic_primitive<bool> terminated;

    /*
     * Number of complete frames received.
     */
    atomic_primitive<uint32_t> rx_frames_;

    /*
     * Number of complete frames transmitted.
     */
    atomic_primitive<uint32_t> tx_frames_;

    /*
     * Number of receive errors.
     */
    atomic_primitive<uint32_t> rx_errors_;

    /*
     * Number of transmit errors.
     */
    atomic_primitive<uint32_t> tx_errors_;

    /*
     * Queue for received serial commands.
     */
    atomic_ringbuffer<serial_command, QUEUE_SIZE> rx_queue_;

    /*
     * Queue for transmitted serial commands.
     */
    atomic_ringbuffer<serial_command, QUEUE_SIZE> tx_queue_;

    /*
     * Pointer to global VEX Brain object.
     */
    const vex::brain *brain_ptr;

    /*
     * ser_thread fields.
     */

    /*
     * The smart port used for serial communication/
     */
    V5_DeviceT smart_port;

    /*
     * The smart port number.
     */
    int32_t port_;

    /*
     * The baudrate of the smart port.
     */
    int32_t baudrate_;

    /*
     * The state of the serial I/O state machine
     */
    serial_state ser_state;

    /*
     * Buffer containing COBS encoded frames received.
     */
    uint8_t rx_buf[RX_BUF_CAP];

    /*
     * Buffer containing COBS encoded frames transmitted.
     */
    uint8_t tx_buf[TX_BUF_CAP];

    /*
     * Buffer containing decoded frames. Since communication is half-duplex,
     * one buffer is sufficient for both transmit and receive.
     */
    uint8_t decoded_buffer[DECODED_BUF_CAP];

    /*
     * The current length of the received COBS encoded frame.
     */
    size_t rx_buf_len;

    /*
     * The timestamp when the frame reception times out.
     */
    uint32_t rx_timeout;
};