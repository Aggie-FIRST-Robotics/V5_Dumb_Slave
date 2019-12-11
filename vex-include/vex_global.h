/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Innovation First 2017-2019, All rights reserved.          */
/*                                                                            */
/*    Module:     vex_global.h                                                */
/*    Author:     James Pearman                                               */
/*    Created:    5 July 2017                                                 */
/*                                                                            */
/*    Revisions:                                                              */
/*                V1.00     TBD - Initial release                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#ifndef   VEX_GLOBAL_H
#define   VEX_GLOBAL_H

/*-----------------------------------------------------------------------------*/
/** @file    vex_global.h
  * @brief   Globals for C++ API
*//*---------------------------------------------------------------------------*/

// more natural language based globals
namespace vex {
    extern rotationUnits       degrees;
    extern rotationUnits       turns;
    extern percentUnits        percent;
    extern timeUnits           seconds;
    extern distanceUnits       inches;
    extern distanceUnits       mm;
    extern directionType       forward;
    extern directionType       reverse;
    extern turnType            left;
    extern turnType            right;

    extern fontType            monoM;
    extern fontType            monoL;
    extern fontType            monoXL;
    extern fontType            monoXXL;
    extern fontType            monoS;
    extern fontType            monoXS;
    extern fontType            propM;
    extern fontType            propL;
    extern fontType            propXL;
    extern fontType            propXXL;

    extern controllerType      primary;
    extern controllerType      partner;

    extern const char *        rumbleLong;
    extern const char *        rumbleShort;
    extern const char *        rumblePulse;

    extern void wait( double time, timeUnits units );
};

namespace vex {
    extern const int32_t       PORT1;
    extern const int32_t       PORT2;
    extern const int32_t       PORT3;
    extern const int32_t       PORT4;
    extern const int32_t       PORT5;
    extern const int32_t       PORT6;
    extern const int32_t       PORT7;
    extern const int32_t       PORT8;
    extern const int32_t       PORT9;
    extern const int32_t       PORT10;
    extern const int32_t       PORT11;
    extern const int32_t       PORT12;
    extern const int32_t       PORT13;
    extern const int32_t       PORT14;
    extern const int32_t       PORT15;
    extern const int32_t       PORT16;
    extern const int32_t       PORT17;
    extern const int32_t       PORT18;
    extern const int32_t       PORT19;
    extern const int32_t       PORT20;
    extern const int32_t       PORT21;
    extern const int32_t       PORT22;
    
    extern percentUnits        pct;
    extern timeUnits           sec;
    extern timeUnits           msec;
    extern voltageUnits        volt;
    extern currentUnits        amp;
    extern powerUnits          watt;
    extern torqueUnits         Nm;
    extern torqueUnits         InLb;
    extern rotationUnits       deg;
    extern rotationUnits       rev;
    extern velocityUnits       rpm;
    extern velocityUnits       dps;
    extern temperatureUnits    celsius;
    extern temperatureUnits    fahrenheit;

    extern directionType       fwd;
    extern brakeType           coast;
    extern brakeType           brake;
    extern brakeType           hold;
    extern gearSetting         ratio36_1;
    extern gearSetting         ratio18_1;
    extern gearSetting         ratio6_1;

    extern const color &       black;
    extern const color &       white;
    extern const color &       red;
    extern const color &       green;
    extern const color &       blue;
    extern const color &       yellow;
    extern const color &       orange;
    extern const color &       purple;
    extern const color &       cyan;
    extern const color &       transparent;

    extern fontType            mono20;
    extern fontType            mono30;
    extern fontType            mono40;
    extern fontType            mono60;
    extern fontType            mono15;
    extern fontType            mono12;
    extern fontType            prop20;
    extern fontType            prop30;
    extern fontType            prop40;
    extern fontType            prop60;

    extern analogUnits         range8bit;
    extern analogUnits         range10bit;
    extern analogUnits         range12bit;
    extern analogUnits         mV;
};

#endif // VEX_GLOBAL_H