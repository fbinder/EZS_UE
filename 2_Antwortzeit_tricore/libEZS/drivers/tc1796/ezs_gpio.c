#include "ezs_gpio.h"

#include <tc1796b.h>
#include <tc1796b/p2.h>

void ezs_gpio_init(void) {
/*
  //configure pins 9 of P2 as general purpose output -> "antrieb 2"
  P2_IOCR8.bits.PC1 = 0x8;
*/

  //configure pins 8 of P2 as general purpose output -> "antrieb 1"
  P2_IOCR8.bits.PC0 = 0x8;
}

void ezs_gpio_set(bool enabled) {
/*
  //pin9 P2
  P2_OUT.bits.P9 = enabled;
*/

  //pin8 P2
  P2_OUT.bits.P8 = enabled;
}

