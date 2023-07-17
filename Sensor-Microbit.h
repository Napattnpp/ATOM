/*
 * Author: Mlld9
 * file: Sensor-Microbit.h
 * version: sm.1
*/

/* subset */

class Sensor {
  private:
    unsigned char NOS = 0;                              // Number of sensor

  public:
    int port[16] = { 0 };
    short val[16] = { 0 };
    int ref[16] = { 0 };
    String val_str = "";

    void init(int _NOS, ...);
    void set_ref(...);
    void gett();
    void conv();
    void conv_str();

    void print_val();
    void print_ref();
    void gett_ref();

};

void Sensor::init(int _NOS, ...) {
  IBIT();

  NOS = _NOS;
  va_list valist;

  va_start(valist, _NOS);                     /* initialize valist for port number of arguments */
  for (unsigned char i = 0; i < _NOS; ++i) {
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
  val_str = "";
  gett();
  for (unsigned char i = 0; i < NOS; ++i) {
    (val[i] < ref[i]) ? val[i] = 1 : val[i] = 0;
    val_str += val[i];
  }
}

/* Show & Print val, ref, conv*/

void Sensor::print_val() {
  for (unsigned char i = 0; i < NOS; ++i) {
    Serial.print("val: ");
    Serial.println(val[i]);
  }
  Serial.println("====================");
}

void Sensor::print_ref() {
  for (unsigned char i = 0; i < NOS; ++i) {
    Serial.print("ref: ");
    Serial.println(ref[i]);
  }
  Serial.println("====================");
}

void Sensor::gett_ref() {
  if (!digitalRead(PIN_BUTTON_A)) {
    fd(10);
    delay(2000);
    fd(0);
    gett();
    print_val();
    for (unsigned char i = 0; i < NOS; ++i) {
      ref[i] = val[i];
    }

    while (1) {
      delay(1500);
      if (!digitalRead(PIN_BUTTON_A)) {
        break;
      }
    }
    fd(10);
    delay(2000);
    fd(0);

    gett();
    print_val();
    for (unsigned char i = 0; i < NOS; ++i) {
      ref[i] = ((ref[i] + val[i]) / 2);
    }
    print_ref();
  }
}
