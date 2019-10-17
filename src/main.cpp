/*
 * Main application file for Dumb-Slave
 *
 * @author John Sauer
 * @date 10/14/2019
 */
#include "vex.h"
#include "serial_thread.h"

/*
 * Port number used for serial communication.
 */
constexpr int32_t port = 19;

/*
 * Baud rate of serial communication.
 */
constexpr int32_t baudrate = 115200;

/*
 * A global instance of vex::brain.
 */
vex::brain brain;

/*
 * Serial thread object.
 */
serial_thread port20_serial;

/*
 * Callback function for serial thread.
 */
void serial20_callback()
{
    port20_serial.serial_routine();
}

/*
 * Main application function.
 */
int main() {

    /*
     * Initialize serial thread.
     */
    port20_serial.init(brain, port, baudrate, serial20_callback);

    /*
     * Loop forever, printing serial statistics and clearing receive command
     * queue.
     */
    while(true)
    {
        brain.Screen.printAt( 10, 50, "Rx frames: %d", port20_serial.rx_frames() );
        brain.Screen.printAt( 10, 100, "Tx frames: %d", port20_serial.tx_frames() );
        brain.Screen.printAt( 10, 150, "Rx errors: %d", port20_serial.rx_errors() );
        brain.Screen.printAt( 10, 200, "Tx errors: %d", port20_serial.tx_errors() );

        while(!port20_serial.rx_queue().empty())
        {
            port20_serial.rx_queue().pop();
        }
        
        vex::this_thread::sleep_for(10);
    }
}
