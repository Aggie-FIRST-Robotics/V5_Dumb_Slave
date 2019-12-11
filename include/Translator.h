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
#include "main.h"

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
    int errors = 0;
    int V5_CODE_VERSION = 1;
    int V5_FIRMWARE_VERSION = 1;
    static constexpr size_t NUM_MOTORS = 19;
    static constexpr size_t NUM_TRIPORT = 7;
    static constexpr int16_t PING = 0;
    static constexpr int16_t GET_V5_FIRMWARE_VERSION = 1;
    static constexpr int16_t GET_V5_CODE_VERSION =2;

    static constexpr int16_t V5_IS_ERRORED = 3;
    static constexpr int16_t V5_ERROR_QUEUE_SIZE=4;
    static constexpr int16_t V5_GET_NEXT_ERROR = 5;
    static constexpr int16_t V5_GET_ERROR_AT_INDEX = 6;

    static constexpr int16_t COMPETITION_IS_ENABLED = 100;
    static constexpr int16_t COMPETITION_IS_DRIVER_CONTROL = 101;
    static constexpr int16_t COMPETITION_IS_AUTONOMOUS = 102;
    static constexpr int16_t COMPETITION_IS_COMPETITION_SWITCH = 103;
    static constexpr int16_t COMPETITION_IS_FIELD_CONTROL = 104;
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
    static constexpr int16_t GET_TX_ERROR = 344;
    static constexpr int16_t GET_RX_ERROR = 345;
    static constexpr int16_t GET_GENERAL_ERROR = 346;
    static constexpr int16_t GET_DEBUG_ERROR = 347;




    static constexpr size_t POOL_SIZE = 1 << 10;

    mem_pool<POOL_SIZE> pool;
    vex::motor* motors[NUM_MOTORS];
    vex::triport* triports[NUM_TRIPORT];
    vex::controller* controllers[2];
    vex::competition* competition;
    vex::brain::battery* battery;


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
        for(size_t x = 0; x < 2; x++){
            auto mem = pool.alloc(sizeof(vex::controller));
            if(x == 0)
            controllers[x] = new(mem) vex::controller{vex::primary};
            else
            controllers[x] = new(mem) vex::controller{vex::partner};
        }
        for(size_t x = 0; x < 1; x++){
            auto mem = pool.alloc(sizeof(vex::competition));
            if(x == 0)
                competition = new(mem) vex::competition{};
            else
                competition = new(mem) vex::competition{};
        }
        for(size_t x = 0; x < 1; x++){
            auto mem = pool.alloc(sizeof(vex::brain::battery));
            if(x == 0)
                battery = new(mem) vex::brain::battery{};
            else
                battery = new(mem) vex::brain::battery{};
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
           return false;


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
                    triports[0]->A.setRotation(payload[x], vex::rotationUnits());

                }
                else if(abs==1){
                    triports[0]->B.setRotation(payload[x], vex::rotationUnits());

                }
                else if(abs==2){
                    triports[0]->C.setRotation(payload[x], vex::rotationUnits());

                }
                else if(abs==3){
                    triports[0]->D.setRotation(payload[x], vex::rotationUnits());

                }
                else if(abs==4){
                    triports[0]->E.setRotation(payload[x], vex::rotationUnits());

                }
                else if(abs==5){
                    triports[0]->F.setRotation(payload[x], vex::rotationUnits());

                }
                else if(abs==6){
                    triports[0]->G.setRotation(payload[x], vex::rotationUnits());

                }
                else if(abs==7){
                    triports[0]->H.setRotation(payload[x], vex::rotationUnits());

                }
            }
        }
        else if(address == PING)
        {
           //Ping should go here, in theory
        }
        else{
            return false;
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
        else if(address==GET_V5_FIRMWARE_VERSION){
            //where is this?
            payload[0] = V5_FIRMWARE_VERSION;


            return true;
        }
        else if(address == GET_V5_CODE_VERSION){

            payload[0] = V5_CODE_VERSION;

            return true;

        }
        else if(address == V5_IS_ERRORED){
            return true;

        }
        else if(address == V5_ERROR_QUEUE_SIZE){
            return true;
            //get error, get tx error, get rx error, getdebug error 4 bytes uint32

        }
        else if(address == V5_GET_NEXT_ERROR){
            return true;

        }
        else if(address == V5_GET_ERROR_AT_INDEX){
            return true;

        }
        else if(address == COMPETITION_IS_ENABLED){
            payload[0] = competition->isEnabled();
            return true;
        }
        else if(address == COMPETITION_IS_DRIVER_CONTROL){
            payload[0] = competition->isDriverControl();
            return true;
        }
        else if(address == COMPETITION_IS_AUTONOMOUS){
            payload[0] = competition->isAutonomous();
            return true;
        }
        else if(address == COMPETITION_IS_COMPETITION_SWITCH){
            payload[0] = competition->isCompetitionSwitch();
            return true;
        }
        else if(address == COMPETITION_IS_FIELD_CONTROL){
            payload[0] = competition->isFieldControl();
            return true;
        }
        else if(address == BATTERY_CAPACITY){
            payload[0] = battery->capacity(vex::percentUnits());
            return true;
        }
        else if(address == BATTERY_TEMPERATURE){
            payload[0] = battery->temperature(vex::temperatureUnits ());
            return true;
        }
        else if(address == BATTERY_VOLTAGE){
            payload[0] = battery->voltage(vex::volt);
            return true;
        }
        else if(address == BATTERY_CURRENT){
            payload[0] = battery->current(vex::amp);
            return true;
        }
        else if(address>=MOTOR_SET_ENCODER_START && address <=MOTOR_SET_ENCODER_END) {
            int abs = address - MOTOR_SET_ENCODER_START;
            //what do we do here?
            return true;


        }
        else if(address>=MOTOR_GET_VELOCITY_START && address <=MOTOR_GET_VELOCITY_END){
            int abs = address-MOTOR_GET_VELOCITY_START;
            payload[0] = motors[abs]->velocity(vex::percentUnits());

            return true;
        }
        else if(address>=MOTOR_GET_CURRENT_START && address <=MOTOR_GET_CURRENT_END){
            int abs = address-MOTOR_GET_CURRENT_START;
            payload[0] = motors[abs]->current(vex::percentUnits());

            return true;
        }
        else if(address>=MOTOR_GET_POWER_START && address <=MOTOR_GET_POWER_END){
            int abs = address-MOTOR_GET_POWER_START;
            payload[0] = motors[abs]->power(vex::powerUnits());

            return true;
        }
        else if(address>=MOTOR_GET_TEMPERATURE_START && address <=MOTOR_GET_TEMPERATURE_END){
            int abs = address-MOTOR_GET_TEMPERATURE_START;
            payload[0] = motors[abs]->temperature(vex::temperatureUnits());

            return true;
        }
        else if(address>=TRIPORT_GET_DIGITAL_START && address <=TRIPORT_GET_DIGITAL_END){
            int abs = address - TRIPORT_GET_DIGITAL_START;
            if(abs==0){
                triports[0]->A.value();

            }
            else if(abs==1){
                triports[0]->B.value();

            }
            else if(abs==2){
                triports[0]->C.value();

            }
            else if(abs==3){
                triports[0]->D.value();

            }
            else if(abs==4){
                triports[0]->E.value();

            }
            else if(abs==5){
                triports[0]->F.value();

            }
            else if(abs==6){
                triports[0]->G.value();

            }
            else if(abs==7){
                triports[0]->H.value();

            }

            return true;
        }

        else if(address>=TRIPORT_GET_ANALOG_START && address <=TRIPORT_GET_ANALOG_END){
            int abs = address - TRIPORT_GET_ANALOG_START;
            if(abs==0){
                triports[0]->A.rotation(vex::rotationUnits());

            }
            else if(abs==1){
                triports[0]->B.rotation(vex::rotationUnits());

            }
            else if(abs==2){
                triports[0]->C.rotation(vex::rotationUnits());

            }
            else if(abs==3){
                triports[0]->D.rotation(vex::rotationUnits());

            }
            else if(abs==4){
                triports[0]->E.rotation(vex::rotationUnits());

            }
            else if(abs==5){
                triports[0]->F.rotation(vex::rotationUnits());

            }
            else if(abs==6){
                triports[0]->G.rotation(vex::rotationUnits());

            }
            else if(abs==7){
                triports[0]->H.rotation(vex::rotationUnits());

            }

            return true;
        }
        else if(address>=CONTROLLER_BUTTON_L1_START && address <=CONTROLLER_BUTTON_L1_END){
            payload[0] = bool (controllers[address-CONTROLLER_BUTTON_L1_START]->ButtonL1.PRESSED);
            return true;
        }
        else if(address>=CONTROLLER_BUTTON_L2_START && address <=CONTROLLER_BUTTON_L2_END){
            payload[0] = bool (controllers[address-CONTROLLER_BUTTON_L2_START]->ButtonL2.PRESSED);
            return true;
        }
        else if(address>=CONTROLLER_BUTTON_R1_START && address <=CONTROLLER_BUTTON_R1_END){
            payload[0] = bool (controllers[address-CONTROLLER_BUTTON_R1_START]->ButtonR1.PRESSED);
            return true;
        }
        else if(address>=CONTROLLER_BUTTON_R2_START && address <=CONTROLLER_BUTTON_R2_END){
            payload[0] = bool ( controllers[address-CONTROLLER_BUTTON_R2_START]->ButtonR2.PRESSED);
            return true;
        }
        else if(address>=CONTROLLER_BUTTON_UP_START && address <=CONTROLLER_BUTTON_UP_END){
            payload[0] = bool ( controllers[address-CONTROLLER_BUTTON_UP_START]->ButtonUp.PRESSED);
            return true;
        }
        else if(address>=CONTROLLER_BUTTON_DOWN_START && address <=CONTROLLER_BUTTON_DOWN_END){
            payload[0] = bool (  controllers[address-CONTROLLER_BUTTON_DOWN_START]->ButtonDown.PRESSED);
            return true;
        }
        else if(address>=CONTROLLER_BUTTON_LEFT_START && address <=CONTROLLER_BUTTON_LEFT_END){
            payload[0] = bool ( controllers[address-CONTROLLER_BUTTON_LEFT_START]->ButtonLeft.PRESSED);
            return true;
        }
        else if(address>=CONTROLLER_BUTTON_RIGHT_START && address <=CONTROLLER_BUTTON_RIGHT_END) {
            payload[0] = bool(controllers[address - CONTROLLER_BUTTON_RIGHT_START]->ButtonRight.PRESSED);
            return true;
        }
        else if(address>=CONTROLLER_BUTTON_X_START && address <=CONTROLLER_BUTTON_X_END){
            payload[0] = bool ( controllers[address-CONTROLLER_BUTTON_X_START]->ButtonX.PRESSED);
            return true;
        }
        else if(address>=CONTROLLER_BUTTON_B_START && address <=CONTROLLER_BUTTON_B_END){
            payload[0] = bool (  controllers[address-CONTROLLER_BUTTON_B_START]->ButtonB.PRESSED);
            return true;
        }
        else if(address>=CONTROLLER_BUTTON_Y_START && address <=CONTROLLER_BUTTON_Y_END){
            payload[0] = bool ( controllers[address-CONTROLLER_BUTTON_Y_START]->ButtonY.PRESSED);
            return true;
        }
        else if(address>=CONTROLLER_BUTTON_A_START && address <=CONTROLLER_BUTTON_A_END) {
            payload[0] = bool(controllers[address - CONTROLLER_BUTTON_A_START]->ButtonA.PRESSED);
            return true;
        }
        else if(address>=CONTROLLER_AXIS_1_START && address <=CONTROLLER_AXIS_1_END){
            payload[0] =  ( controllers[address-CONTROLLER_AXIS_1_START]->Axis1.value());
            return true;
        }
        else if(address>=CONTROLLER_AXIS_2_START && address <=CONTROLLER_AXIS_2_END){
            payload[0] =  ( controllers[address-CONTROLLER_AXIS_2_END]->Axis2.value());
            return true;
        }
        else if(address>=CONTROLLER_AXIS_3_START && address <=CONTROLLER_AXIS_3_END){
            payload[0] =  ( controllers[address-CONTROLLER_AXIS_3_END]->Axis3.value());
            return true;
        }
        else if(address>=CONTROLLER_AXIS_4_START && address <=CONTROLLER_AXIS_4_END) {
            payload[0] =  ( controllers[address-CONTROLLER_AXIS_4_END]->Axis4.value());
            return true;
        }
        else if(address==GET_TX_ERROR) {
            payload[0] = (port20_serial.tx_errors());
            return true;
        }
        else if(address==GET_RX_ERROR) {
            payload[0] = (port20_serial.rx_errors());

            return true;
        }
        else if(address==GET_GENERAL_ERROR) {
            payload[0] = (errors);
            errors++;
            return true;
        }
        else if(address==GET_DEBUG_ERROR) {
            return false;        }
        else{
            return false;
        }





        //get from table



    }
};
#endif //V5_DUMB_SLAVE_TRANSLATOR_H
