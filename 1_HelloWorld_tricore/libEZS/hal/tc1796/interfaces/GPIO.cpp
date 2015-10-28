#include "GPIO.h"

#include <tc1796b/p0.h>
#include <tc1796b/p1.h>
#include <tc1796b/p2.h>
#include <tc1796b/p3.h>
#include <tc1796b/p4.h>
#include <tc1796b/p5.h>
#include <tc1796b/p6.h>
#include <tc1796b/p7.h>
#include <tc1796b/p8.h>
#include <tc1796b/p9.h>

#define GPIO_PORT_IN_ADDR(port) (P0_IN_ADDR + port * 0x0100)
#define GPIO_PORT_OUT_ADDR(port) (P0_OUT_ADDR + port * 0x0100)

void GPIO::init()
{
    /*
     * set all pins to "output" and "low"
     */
     return; //debug
    unsigned char i, j;
    for (i = 0; i <= 9; i++) {
        if ( i == 4 ) {
            continue;
        }
        for (j = 0; j < 16; j++) {
            setFunction(i, j, GPIO_FUNCTION_INPUT_PULLDOWN);
        }
    }
}


void GPIO::setOutput(uint8_t port, uint8_t pin, uint8_t out)
{
    unsigned long *outreg = (unsigned long *) GPIO_PORT_OUT_ADDR(port);
    if (out) {
        // set output
        *outreg |= (1 << pin);
    } else {
        // clear output
        *outreg &= ~(1 << pin);
    }
}


int GPIO::getInput(uint8_t port, uint8_t pin)
{
	unsigned long *inreg = (unsigned long *) GPIO_PORT_IN_ADDR(port);
	return (*inreg & (1 << pin))!=0;
}


int GPIO::getOutput(uint8_t port, uint8_t pin)
{
    unsigned long *outreg = (unsigned long *) GPIO_PORT_OUT_ADDR(port);
    return (*outreg & (1 << pin))!=0;
}


bool GPIO::setFunction(uint8_t port, uint8_t pin, uint8_t function)
{
    unsigned char iocr_number =  pin >> 2;
    unsigned long iocr_mask = 0x000000f0 << (8 * (pin & 3));
    unsigned long iocr_val  = (function & 15) << (4 + 8 * (pin & 3));
    unsigned long *iocr;
    switch (port) {
        case  0:
            switch (iocr_number) {
                case 0: iocr = (unsigned long *) P0_IOCR0_ADDR; break;
                case 1: iocr = (unsigned long *) P0_IOCR4_ADDR; break;
                case 2: iocr = (unsigned long *) P0_IOCR8_ADDR; break;
                case 3: iocr = (unsigned long *) P0_IOCR12_ADDR; break;
                default: return false;
            }
            break;
        case  1:
            switch (iocr_number) {
                case 0: iocr = (unsigned long *) P1_IOCR0_ADDR; break;
                case 1: iocr = (unsigned long *) P1_IOCR4_ADDR; break;
                case 2: iocr = (unsigned long *) P1_IOCR8_ADDR; break;
                case 3: iocr = (unsigned long *) P1_IOCR12_ADDR; break;
                default: return false;
            }
            break;
        case  2:
            switch (iocr_number) {
                case 0: iocr = (unsigned long *) P2_IOCR0_ADDR; break;
                case 1: iocr = (unsigned long *) P2_IOCR4_ADDR; break;
                case 2: iocr = (unsigned long *) P2_IOCR8_ADDR; break;
                case 3: iocr = (unsigned long *) P2_IOCR12_ADDR; break;
                default: return false;
            }
            break;
        case  3:
            switch (iocr_number) {
                case 0: iocr = (unsigned long *) P3_IOCR0_ADDR; break;
                case 1: iocr = (unsigned long *) P3_IOCR4_ADDR; break;
                case 2: iocr = (unsigned long *) P3_IOCR8_ADDR; break;
                case 3: iocr = (unsigned long *) P3_IOCR12_ADDR; break;
                default: return false;
            }
            break;
        case  4:
            switch (iocr_number) {
                case 0: iocr = (unsigned long *) P4_IOCR0_ADDR; break;
                case 1: iocr = (unsigned long *) P4_IOCR4_ADDR; break;
                case 2: iocr = (unsigned long *) P4_IOCR8_ADDR; break;
                case 3: iocr = (unsigned long *) P4_IOCR12_ADDR; break;
                default: return false;
            }
            break;
        case  5:
            switch (iocr_number) {
                case 0: iocr = (unsigned long *) P5_IOCR0_ADDR; break;
                case 1: iocr = (unsigned long *) P5_IOCR4_ADDR; break;
                default: return false;
            }
            break;
        case  6:
            switch (iocr_number) {
                case 1: iocr = (unsigned long *) P6_IOCR4_ADDR; break;
                case 2: iocr = (unsigned long *) P6_IOCR8_ADDR; break;
                case 3: iocr = (unsigned long *) P6_IOCR12_ADDR; break;
                default: return false;
            }
            break;
        case  7:
            switch (iocr_number) {
                case 0: iocr = (unsigned long *) P7_IOCR0_ADDR; break;
                case 1: iocr = (unsigned long *) P7_IOCR4_ADDR; break;
                default: return false;
            }
            break;
        case  8:
            switch (iocr_number) {
                case 0: iocr = (unsigned long *) P8_IOCR0_ADDR; break;
                case 1: iocr = (unsigned long *) P8_IOCR4_ADDR; break;
                default: return false;
            }
            break;
        case  9:
            switch (iocr_number) {
                case 0: iocr = (unsigned long *) P9_IOCR0_ADDR; break;
                case 1: iocr = (unsigned long *) P9_IOCR4_ADDR; break;
                case 2: iocr = (unsigned long *) P9_IOCR8_ADDR; break;
                default: return false;
            }
            break;
        default: return false;
    }

    *iocr &= ~iocr_mask;
    *iocr |= iocr_val;
    return true;
}
