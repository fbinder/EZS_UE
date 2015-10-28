#include "DAC.h"

#include "GPTA.h"
#include "GPIO.h"
#include "IOLS.h"
#include "LTC.h"
#include "OSAbstraction.h"

#define PERIOD 0xfff
#define MIN 0x058 //minimal value. below dac outputs zero
#define MAX 0xff0 //maximal value. above dac outputs full duty cycle

void DAC::init(unsigned char port, unsigned char baseLtc)
{
    
    this->ltc = baseLtc;
    this->port = port;
    
    GPIO::setFunction(IO_PORT(port), IO_PIN(port), GPIO_FUNCTION_GPTA0);

    //configure the connects
    if ( !IOLS<0>::connectLTCInputClock(baseLtc, 0) )
    {
        osPanic();    //main clock to the first ltc
    }
    if ( !IOLS<0>::connectIoLTC(port, baseLtc+2) )
    {
        osPanic();    //wiring the output pin with the last ltc (base+2)
    }
    //initializing the counting time for comparing
    LTC<0>::setupResetTimer(baseLtc);

    //Timer for the main period
    LTC<0>::setupComparator(baseLtc+1, PERIOD, LTC_SO_BOTH);
    LTC<0>::setComparatorAction(baseLtc+1, LTC_COMPARATOR_ACTION_SET, LTC_EVENT_LOCAL);

    //Timer for the high-period
    LTC<0>::setupComparator(baseLtc+2, 0, LTC_SO_BOTH);
    LTC<0>::setComparatorAction(baseLtc+2, LTC_COMPARATOR_ACTION_RESET, LTC_EVENT_ALL);
    on();
}

void DAC::on() {
    if ( ! state ) {
        LTC<0>::setX(ltc+2, 0);
        GPIO::setFunction(IO_PORT(port), IO_PIN(port), GPIO_FUNCTION_GPTA0);
        state = true;
    }
}

void DAC::off()
{
    if ( state ) {
        LTC<0>::setX(ltc+2, 0);
        GPIO::setFunction(IO_PORT(port), IO_PIN(port), GPIO_FUNCTION_INPUT_PULLDOWN);
        state = false;
    }
}

void DAC::write(uint8_t v)
{
  uint16_t value = 0;
  if(v == 0) {
    value = 0;
  }
  else if (v == UINT8_MAX) {
    value = PERIOD-1;
  }
  else {
    uint16_t diff = MAX-MIN;
    float frac = (float)v / (float) UINT8_MAX;
    uint16_t mapped = frac * (float) diff;
    value = MIN + mapped;
  }
  LTC<0>::setX(ltc+2, value);
}
