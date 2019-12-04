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
#include "v5_api.h"
#include "vex_motor.h"

template<size_t L>
class mem_pool{
    uint8_t pool[L]{};
    size_t tracker = 0;

public:
    mem_pool() = default;

    void* alloc(size_t size){
        tracker += size;
        return pool + tracker - size;
    }
};

class Translator{
    static constexpr size_t NUM_MOTORS = 19;
    static constexpr size_t NUM_TRIPORT = 7;
    static constexpr int16_t Ping = 0;
    static constexpr int16_t GET_V5_FIRMWARE_VERSION = 1;
    static constexpr int16_t V5_IS_ERRORED = 3;
    static constexpr int16_t V5_ERROR_QUEUE_SIZE=4;
    static constexpr int16_t V5_GET_NEXT_ERROR = 5;
    static constexpr int16_t COMPETITION_IS_ENABLED = 100;
    static constexpr int16_t COMPETITION_IS_AUTONOMOUS = 102;
    static constexpr int16_t COMPETITION_IS_COMPETITION_SWITCH = 103;
    static constexpr int16_t COMPETITION_IS_FIELD_CONTROL = 104
    static constexpr int16_t BATTERY_CAPACITY = 105;
    static constexpr int16_t BATTERY_TEMPERATURE = 106;
    static constexpr int16_t BATTERY_VOLTAGE = 107;
    static constexpr int16_t BATTERY_CURRENT = 108;
    static constexpr int16_t MOTOR_REVERSE_START =109;
    static constexpr int16_t MOTOR_REVERSE_END =127;
    static constexpr int16_t MOTOR_BRAKE_MODE_START =128;
    static constexpr int16_t MOTOR_BRAKE_MODE_END =146;
    static constexpr int16_t MOTOR_SET_ENCODER_START =147;
    static constexpr int16_t MOTOR_SET_ENCODER_END =165;
    static constexpr int16_t MOTOR_SET_VOLTAGE_START =166;
    static constexpr int16_t MOTOR_SET_VOLTAGE_END =184;
    static constexpr int16_t MOTOR_GET_ENCODER_START =185;
    static constexpr int16_t MOTOR_GET_ENCODER_END =203;

    static constexpr int16_t MOTOR_GET_VELOCITY_START =204;
    static constexpr int16_t MOTOR_GET_VELOCITY_END =222;
    static constexpr int16_t MOTOR_GET_CURRENT_START =223;
    static constexpr int16_t MOTOR_GET_CURRENT_END =241;
    static constexpr int16_t MOTOR_GET_POWER_START =242;
    static constexpr int16_t MOTOR_GET_POWER_END =260;
    static constexpr int16_t MOTOR_GET_TEMPERATURE_START =261;
    static constexpr int16_t MOTOR_GET_TEMPERATURE_END =279;
    static constexpr int16_t MOTOR_GET_DIGITAL_START =280;
    static constexpr int16_t MOTOR_GET_DIGITAL_END =287;
    static constexpr int16_t MOTOR_SET_DIGITAL_START =288;
    static constexpr int16_t MOTOR_SET_DIGITAL_END =295;
    static constexpr int16_t TRIPORT_GET_DIGITAL_START = 280;
    static constexpr int16_t TRIPORT_GET_DIGITAL_END = 287;
    static constexpr int16_t TRIPORT_SET_DIGITAL_START = 288;
    static constexpr int16_t TRIPORT_SET_DIGITAL_END = 295;
    static constexpr int16_t TRIPORT_GET_ANALOG_START = 296;
    static constexpr int16_t TRIPORT_GET_ANALOG_END = 303;
    static constexpr int16_t TRIPORT_SET_PWM_START = 304;
    static constexpr int16_t TRIPORT_SET_PWM_END = 311;
    static constexpr int16_t CONTROLLER_BUTTON_L1_START = 312;
    static constexpr int16_t CONTROLLER_BUTTON_L1_END = 313;
    static constexpr int16_t CONTROLLER_BUTTON_L2_START = 314;
    static constexpr int16_t CONTROLLER_BUTTON_L2_END = 315;
    static constexpr int16_t CONTROLLER_BUTTON_R1_START = 316;
    static constexpr int16_t CONTROLLER_BUTTON_R1_END = 317;
    static constexpr int16_t CONTROLLER_BUTTON_R2_START = 318;
    static constexpr int16_t CONTROLLER_BUTTON_R2_END = 319;
    static constexpr int16_t CONTROLLER_BUTTON_DOWN_START = 322;
    static constexpr int16_t CONTROLLER_BUTTON_DOWN_END = 323;
    static constexpr int16_t CONTROLLER_BUTTON_UP_START = 320;
    static constexpr int16_t CONTROLLER_BUTTON_UP_END = 321;
    static constexpr int16_t CONTROLLER_BUTTON_LEFT_START = 324;
    static constexpr int16_t CONTROLLER_BUTTON_LEFT_END = 325;
    static constexpr int16_t CONTROLLER_BUTTON_RIGHT_START = 326;
    static constexpr int16_t CONTROLLER_BUTTON_RIGHT_END = 327;
    static constexpr int16_t CONTROLLER_BUTTON_X_START = 328;
    static constexpr int16_t CONTROLLER_BUTTON_X_END = 329;
    static constexpr int16_t CONTROLLER_BUTTON_B_START = 330;
    static constexpr int16_t CONTROLLER_BUTTON_B_END = 331;
    static constexpr int16_t CONTROLLER_BUTTON_Y_START = 332;
    static constexpr int16_t CONTROLLER_BUTTON_Y_END = 333;
    static constexpr int16_t CONTROLLER_BUTTON_A_START = 334;
    static constexpr int16_t CONTROLLER_BUTTON_A_END = 335;
    static constexpr int16_t CONTROLLER_AXIS_1_START = 336 ;
    static constexpr int16_t CONTROLLER_AXIS_1_END = 337;
    static constexpr int16_t CONTROLLER_AXIS_2_START = 338;
    static constexpr int16_t CONTROLLER_AXIS_2_END = 339;
    static constexpr int16_t CONTROLLER_AXIS_3_START = 340;
    static constexpr int16_t CONTROLLER_AXIS_3_END = 341;
    static constexpr int16_t CONTROLLER_AXIS_4_START = 342;
    static constexpr int16_t CONTROLLER_AXIS_4_END = 343;




    static constexpr size_t POOL_SIZE = 1 << 10;

    mem_pool<POOL_SIZE> pool;
    vex::motor* motors[NUM_MOTORS];
    vex::triport* triports[NUM_TRIPORT];

public:

    Translator(){
        for(size_t x = 0; x < NUM_MOTORS; x++){
            auto mem = pool.alloc(sizeof(vex::motor));
            motors[x] = new(mem) vex::motor{int32_t(x)};
        }
        for(size_t x = 0; x < NUM_TRIPORT; x++){
            auto mem = pool.alloc(sizeof(vex::triport));
            triports[x] = new(mem) vex::triport{int32_t(x)};
        }

    }
/**
 * Sets the system corresponding to the address to the payload data.
 *
 * @param address The address being written to.
 * @param payload The array containing the payload. Data is big endian.
 * @param payload_len The length of the payload.
 * @return True if successful, false if not.
 *
 */
    bool write_command(uint16_t address, uint8_t *payload, uint8_t payload_len)
    {
        if(address>=0x8000)
            return false;
        else if(address>=MOTOR_REVERSE_START && address <=MOTOR_REVERSE_END){
            int abs = address-MOTOR_REVERSE_START;
            for(int x = 0; x<payload_len; x++)
                motors[abs]->setReversed(payload[x]);
            return true;
        }
        else if(address>=MOTOR_REVERSE_START && address <=MOTOR_REVERSE_END) {
            int abs = address - MOTOR_REVERSE_START;
            for (int x = 0; x < payload_len; x++) {
                vexDeviceMotorBrakeModeSet(vexDeviceGetByIndex(abs), _V5_MotorBrakeMode(payload[x]));
                if(payload[x]==0)
                motors[abs]->setBrake(vex::coast);
                else if(payload[x]==1)
                    motors[abs]->setBrake(vex::brake);
            }
            return true;

        }
       else if(address>=MOTOR_SET_ENCODER_START && address <=MOTOR_SET_ENCODER_END) {
           /*
            int abs = address - MOTOR_SET_ENCODER_START;
            for (int x = 0; x < payload_len; x++) {
                motors[abs]->enco
                vexDeviceMotorEncoderUnitsSet(vexDeviceGetByIndex(abs), payload[x]);
            }
            return true;

            */

        }
        else if(address>=MOTOR_SET_VOLTAGE_START && address <=MOTOR_SET_VOLTAGE_END) {
            int abs = address - MOTOR_SET_VOLTAGE_START;
            for (int x = 0; x < payload_len; x++) {
                motors[abs]->spin(vex::forward, payload[x], vex::volt);
            }
            return true;


        }
        else if(address>=MOTOR_SET_DIGITAL_START && address <=MOTOR_SET_DIGITAL_END) {

            int abs = address - MOTOR_SET_DIGITAL_START;
            for (int x = 0; x < payload_len; x++) {
                if(abs==0){
                    triports[0]->A.set(payload[x]);

                }
                else if(abs==1){
                    triports[0]->B.set(payload[x]);

                }
                else if(abs==2){
                    triports[0]->C.set(payload[x]);

                }
                else if(abs==3){
                    triports[0]->D.set(payload[x]);

                }
                else if(abs==4){
                    triports[0]->E.set(payload[x]);

                }
                else if(abs==5){
                    triports[0]->F.set(payload[x]);

                }
                else if(abs==6){
                    triports[0]->G.set(payload[x]);

                }
                else if(abs==7){
                    triports[0]->H.set(payload[x]);

                }
                }
            }

        else if(address >=TRIPORT_SET_PWM_START && address <=TRIPORT_SET_PWM_END){
            int abs = address - TRIPORT_SET_PWM_START;
            for (int x = 0; x < payload_len; x++) {
                if(abs==0){
                    triports[0]->A.setRotation(payload[x]);

                }
                else if(abs==1){
                    triports[0]->B.setRotation(payload[x]);

                }
                else if(abs==2){
                    triports[0]->C.setRotation(payload[x]);

                }
                else if(abs==3){
                    triports[0]->D.setRotation(payload[x]);

                }
                else if(abs==4){
                    triports[0]->E.setRotation(payload[x]);

                }
                else if(abs==5){
                    triports[0]->F.setRotation(payload[x]);

                }
                else if(abs==6){
                    triports[0]->G.setRotation(payload[x]);

                }
                else if(abs==7){
                    triports[0]->H.setRotation(payload[x]);

                }
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
        if(address<0x8000)
            return false;
        //get from table



    }
};
#endif //V5_DUMB_SLAVE_TRANSLATOR_H
