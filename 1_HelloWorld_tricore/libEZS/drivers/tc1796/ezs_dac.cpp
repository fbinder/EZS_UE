/*!
 * @file ezs_dac.c
 * @brief A C wrapper for the tricore hal DAC driver
 * @author Tobias Klaus
 * \ingroup dac 
 */


#include "ezs_dac.h"
#include "DAC.h"
#include "GPIO.h"
#include "GPTA.h"
#include "SystemFrequency.h"

static DAC dac = DAC();


void ezs_dac_init() {
  SystemFrequency::Instance().init();
  GPIO::init();
  GPTA::init(10000000);
  GPTA::initModule(0);
  dac.init(0, 1);//0^=P2.8, 1 =^ P2.9
}

void ezs_dac_write(uint8_t value) {
  dac.write(value);
}
