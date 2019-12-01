/*
 * This header contains the classes that communicates between the V5 and the Serial Communicator.
 *
 * @author Aamhish Rao
 * @date 11/26/2019
 */

#ifndef V5_DUMB_SLAVE_TRANSLATOR_H
#define V5_DUMB_SLAVE_TRANSLATOR_H
#include "vex.h"
#include <stdint-gcc.h>
#include <vector>

class Translator{
std::vector<int16_t> payload;

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
        if(address>=8000)
            return 0;
        else if(address>=109 && address <=127){
            int abs = address-109;
            for(int x = 0; x<payload_len; x++)
            vexDeviceMotorReverseFlagSet(vexDeviceGetByIndex(abs), payload[x]);

        }
        else if(address>=128 && address <=146) {
            int abs = address - 128;
            for (int x = 0; x < payload_len; x++) {
                vexDeviceMotorBrakeModeSet(vexDeviceGetByIndex(abs), _V5_MotorBrakeMode(payload[x]));
            }
        }
       else if(address>=147 && address <=165) {
            int abs = address - 147;
            for (int x = 0; x < payload_len; x++) {
                vexDeviceMotorEncoderUnitsSet(vexDeviceGetByIndex(abs), payload[x]);
            }

        }
        else if(address>=166 && address <=184) {
            int abs = address - 166;
            for (int x = 0; x < payload_len; x++) {
                vexDeviceMotorVoltageSet(vexDeviceGetByIndex(abs), payload[x]);
            }

        }
        else if(address>=288 && address <=295) {

            //no idea what to do here

        }
        else if(address >=304 && address <=311){
            int abs = address - 304;
            for (int x = 0; x < payload_len; x++) {
                vexDeviceAdiValueSet(vexDeviceGetByIndex(abs), abs, payload[x]);
            }
        }





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
        if(address<8000)
            return 0;
        //get from table



    }
};
#endif //V5_DUMB_SLAVE_TRANSLATOR_H
