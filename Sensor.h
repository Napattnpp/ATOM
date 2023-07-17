/*
 * Author: Mlld9
 * file: Sensor.h
 * version: s.5.1
*/

/* subset */

class Sensor {
  private:
    unsigned char address = 0;                          // eeprom address

  public:
    unsigned char NOS = 0;                              // Number of sensor
    int port[16] = { 0 };
    int val[16] = { 0 };
    int ref[16] = { 0 };
    char val_str[16] = { "\0" };

    void init(int nos, ...);
    void set_ref(...);

    void gett();
    void conv();
    void conv_str();

    void print_val();
    void show_val();

    void print_ref();
    void show_ref();

    #if defined(HAVE_EEPROM)
      void gett_ref();
      void update_ref();
    #endif
};

/* public */

void Sensor::init(int nos, ...) {
  #if defined(ARDUINO_AVR_ATX2M)
   XIO();
  #endif

  NOS = nos;
  va_list valist;

  va_start(valist, NOS);                     /* initialize valist for port number of arguments */
  for (unsigned char i = 0; i < NOS; ++i) {
    port[i] = va_arg(valist, int);
  }
  va_end(valist);
}

void Sensor::set_ref(...) {
  va_list valist;

  va_start(valist, NOS);                     /* initialize valist for port number of arguments */
  for (unsigned char i = 0; i < NOS; ++i) {
    ref[i] = va_arg(valist, int);
  }
  va_end(valist);
}

void Sensor::gett() {
  for (unsigned char i = 0; i < NOS; ++i) {
    val[i] = analog(port[i]);
  }
}

void Sensor::conv() {
  gett();
  for (unsigned char i = 0; i < NOS; ++i) {
    (val[i] < ref[i]) ? val[i] = 1 : val[i] = 0;
  }
}

void Sensor::conv_str() {
  //val_str[0] = '\0';
  gett();
  for (unsigned char i = 0; i < NOS; ++i) {
  val_str[i] = (val[i] < ref[i]) ? '1' : '0';
  }
}

/* Show & Print val, ref, conv */

void Sensor::print_val() {
  for (unsigned char i = 0; i < NOS; ++i) {
    Serial.print("val: ");
    Serial.println(val[i]);
  }
  Serial.println("====================");
}

void Sensor::show_val() {
  for (unsigned char i = 0; i < NOS; ++i) {
    SCREEN_TEXT(i, 0, "val[%d]: %d    ", i, val[i]);
    SCREEN_SHOW;
  }
}

void Sensor::print_ref() {
  for (unsigned char i = 0; i < NOS; ++i) {
    Serial.print("ref: ");
    Serial.println(ref[i]);
  }
  Serial.println("====================");
}

void Sensor::show_ref() {
  for (unsigned char i = 0; i < NOS; ++i) {
    SCREEN_TEXT(i, 0, "ref: %d    ", ref[i]);
    SCREEN_SHOW;
  }
}

#if defined(HAVE_EEPROM)
void Sensor::gett_ref() {
  address = 0;

  /* get Ref1 */
  SCREEN_TEXT(0, 0, "Ref 1");
  SCREEN_SHOW;
  sw_ok_press();

  gett();
  for (unsigned char i = 0; i < NOS; ++i) {
    ref[i] = val[i];
  }
  for (unsigned char i = 0; i < NOS; ++i) {
    SCREEN_TEXT(i + 1, 0, "val %d: %d    ", i, ref[i]);
    SCREEN_SHOW;
  }

  sw_ok_press();
  SCREEN_CLEAR;
  /* End get Ref1 & clear screen */

  /* get Ref2 */
  SCREEN_TEXT(0, 0, "Ref 2");
  SCREEN_SHOW;
  sw_ok_press();

  gett();
  for (unsigned char i = 0; i < NOS; ++i) {
    SCREEN_TEXT(i + 1, 0, "val %d: %d    ", i, val[i]);
    SCREEN_SHOW;
  }

  sw_ok_press();
  SCREEN_CLEAR;
  /* End get Ref2 & clear screen */

  /* get Result */
  SCREEN_TEXT(0, 0, "Result");
  SCREEN_SHOW;
  sw_ok_press();

  for (unsigned char i = 0; i < NOS; ++i) {
    //glcd(0, 0, "Address: %d", address);

    ref[i] = (ref[i] + val[i])/2;
    #if defined(HAVE_EEPROM)
      EEPROM.put(address, ref[i]);
      address += (sizeof(int));
    #endif

  }
  for (unsigned char i = 0; i < NOS; ++i) {
    SCREEN_TEXT(i + 1, 0, "ref: %d    ", ref[i]);
    SCREEN_SHOW;
  }

  sw_ok_press();
  SCREEN_CLEAR;
  /* clear screen */
}

void Sensor::update_ref() {
  address = 0;

  if (sw_ok()) {
    gett_ref();
  } else {
    for (unsigned char i = 0; i < NOS; ++i) {
      //glcd(0, 0, "Address: %d", address);
      
      EEPROM.get(address, ref[i]);
      address += (sizeof(int));
    }
  }
}
#endif
