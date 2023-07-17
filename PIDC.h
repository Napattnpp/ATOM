/*
 * Author: Mlld9
 * file: PIDC.h
 * version: p.5.4
*/

class PIDC {
    private:
        unsigned n = 0;
        unsigned last = 0;
        unsigned pre = 0;
        unsigned s = 0;
        unsigned char i_char = 0;
        unsigned char i_str = 0;

        // tune.
        int arrow = -1;
        TYPE X = 0;
        TYPE pre_X = 0;

        void update_situa(unsigned n);

    public:
        Sensor sen;
        unsigned char NOS = 0;
        char situa[16][16] = { "\0",
                               "\0"
                             };

        int err_list[16] = { 0 };

        int err = 0;
        int pre_err = 0;
        int sum_err = 0;

        TYPE kp = 0;
        TYPE ki = 0;
        TYPE kd = 0;
        TYPE result = 0;

        void init(unsigned char _NOS);
        void set_err_list(...);
        int comp(char str1[], char str2[]);
        void gett_err();                                            //get error.
        void ces();                               // calulate the error situation.
        void start(void (*func)());
        void tune(void (*func)());
};

//? f2 ext1(+) int1(+)
void PIDC::update_situa(unsigned n) {
    //convert to binary.
    i_char = 0;

    for (unsigned i = 1 << (NOS - 1); i > 0; i = i >> 1) {
        situa[i_str][i_char] = (n & i) ? '1' : '0';
        ++i_char;
        //Serial.print((n & i) ? '1' : '0');
    }
    ++i_str;
    //Serial.println("  ");
}

int PIDC::comp(char str1[], char str2[]) {
  int acl = 0;
  for (int i = 0; i < NOS; i++) {
    if (str1[i] == str2[i]) {
      acl++;
    } else {
      acl--;
      if (acl < 0) { break; }
    }
  }
  if (acl == NOS) {
    return 1;
  } else {
    return 0;
  }
}

void PIDC::init(unsigned char _NOS) {
    NOS = _NOS;
}

//? ces1();
void PIDC::ces() {
    for (unsigned cur = 1; cur != (1 << NOS); cur = cur << 1) {
        if (s) {
            pre = (last | cur);                                    // get [orn].
            update_situa(pre);
        } else {
            s = !s;
        }
        update_situa(cur);                                          // get shn.

        last = cur;
    }
}

void PIDC::set_err_list(...) {
    int _NOS = ((NOS*2) - 1);
    va_list valist;

    va_start(valist, _NOS);
    for (unsigned char i = 0; i < _NOS; ++i) {
        err_list[i] = va_arg(valist, int);
    }
    va_end(valist);
} 

void PIDC::gett_err() {
    // get current error stuation.
    sen.conv_str();

    /*
     * compare current error situation with seted error situation
     * and set a error value.
    */
    for (unsigned char i = 0; i < ((NOS*2) - 1); ++i) {
        if (comp(sen.val_str, situa[i])) {
            err = err_list[i];
            // Serial.println(sen.val_str);
            // Serial.println(situa[i]);
            //Serial.println(err);
            break;
        }
    }
}

void PIDC::start(void (*func)()) {
    //SCREEN_TEXT(6, 0, "res p er d per i ser"); SCREEN_SHOW;
    gett_err();
    result = ((kp*err) + (ki*sum_err) + (kd*(err-pre_err)));
    //SCREEN_TEXT(7, 0, "%d  %d  %d  %d  %d  %d  %d  ", result, kp, err, kd, pre_err, ki, sum_err); SCREEN_SHOW;
    //Serial.println(result);

    (func)();

    pre_err = err;
    sum_err += err;
}

void PIDC::tune(void (*func)()) {
  // show default PID value.
  SCREEN_CLEAR;
  SCREEN_TEXT(0, 4, "kp:");
  SCREEN_TEXT(1, 4, "ki:");
  SCREEN_TEXT(2, 4, "kd:");
  SCREEN_SHOW;

  while (1) {
    // if button is pressed mode will change.
    if (sw_ok()) {
        SCREEN_TEXT(arrow-1, 0, "    "); SCREEN_SHOW;
        SCREEN_TEXT(arrow, 0, "--> "); SCREEN_SHOW;

        ++arrow;
        if (arrow == 4) { SCREEN_TEXT(arrow-1, 0, "    "); SCREEN_SHOW; arrow = 0;}

        //SCREEN_TEXT(5, 0, "arrow: %d  ", arrow); SCREEN_SHOW;
        
        delay(300);
    }

    X = knob();
    X = map(X, 0, MAX_ANALOG, 0, 50);

    // update PID value and glcd.
    // if mode is same not update glcd.
    if (pre_X != X) {
        if (arrow == 1) {
            kp = X;
            SCREEN_TEXT(0, 8, PFM, kp);    SCREEN_SHOW;
        } else if (arrow == 2) {
            ki = X;
            SCREEN_TEXT(1, 8, PFM, ki);    SCREEN_SHOW;
        } else if (arrow == 3) {
            kd = X;
            SCREEN_TEXT(2, 8, PFM, kd);    SCREEN_SHOW;
        }

    }

    if (arrow == 0) {
        start(func);
    } else {
        AO();
        // SCREEN_TEXT(6, 0, "                    "); SCREEN_SHOW;
        // SCREEN_TEXT(7, 0, "                    "); SCREEN_SHOW;
    }

    pre_X = X;
  }
}
