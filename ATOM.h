/*
 * Author: Mlld9
 * file: ATOM.h
 * version: a.4
*/

#include <stdarg.h>

#include "init.h"

/* --- Board Library. --- */
#if defined(ARDUINO_AVR_ATX2M)
  #include <ATX2.h>
#elif defined(ARDUINO_AVR_POP)
  #include <pop7.h>
#elif defined(ARDUINO_AVR_IPST)
  #include <ipst.h>
#elif defined(ARDUINO_BBC_MICROBIT_V2) || defined(ARDUINO_ARCH_NRF5)
  #include <IBIT.h>
#elif defined(ARDUINO_GENERIC_F103CBTX)
  #include <POP32.h>
#elif defined(ARDUINO_AVR_UNO)
  // tester.
#else
  #error Not-Define
#endif

#if defined(HAVE_EEPROM)
  #include <EEPROM.h>
#endif

#if defined(ARDUINO_AVR_ATX2M) || defined(ARDUINO_AVR_POP) || defined(ARDUINO_AVR_IPST) || defined(ARDUINO_GENERIC_F103CBTX)
  #include "Sensor.h"
  #include "PIDC.h"
#elif defined(ARDUINO_BBC_MICROBIT_V2) || defined(ARDUINO_ARCH_NRF5)
  #include "Sensor-Microbit.h"
#endif
