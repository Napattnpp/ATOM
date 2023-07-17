/*
 * Author: Mlld9
 * file: init.h
 * version: i.5.2
*/

#if defined(ARDUINO_AVR_ATX2M) || defined(ARDUINO_AVR_POP) || defined(ARDUINO_AVR_IPST)
  //screen.
  #define SCREEN_TEXT(...)  glcd(__VA_ARGS__);
  #define SCREEN_SHOW
  #define SCREEN_CLEAR      glcdClear();

  //button.
  #define ISREADY OK();

  //PID
  #define TYPE              int
  #define PFM               "%d  "
  #define MAX_ANALOG        1023

#elif defined(ARDUINO_GENERIC_F103CBTX)
  //screen.
  #define SCREEN_TEXT(...)  oled.text(__VA_ARGS__);
  #define SCREEN_SHOW       oled.show();
  #define SCREEN_CLEAR      oled.clear();

  //button.
  #define ISREADY waitSW_OK_bmp();

  //PID
  #define TYPE              float
  #define PFM               "%f      "
  #define MAX_ANALOG        4000

#endif

//EEPROM
#if defined(ARDUINO_AVR_ATX2M) || defined(ARDUINO_AVR_POP) || defined(ARDUINO_AVR_IPST) || defined(ARDUINO_GENERIC_F103CBTX)
  #define HAVE_EEPROM
#elif defined(ARDUINO_BBC_MICROBIT_V2) || defined(ARDUINO_ARCH_NRF5)
  #define NO_EEPROM
#endif
