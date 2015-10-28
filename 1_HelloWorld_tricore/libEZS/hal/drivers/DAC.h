/*====================================================================
* Function: Driver for generell PWM with GPTA
*
* Copyright I4Copter Project 2007-2011
* http://www4.informatik.uni-erlangen.de/Research/I4Copter
*=====================================================================
* Author: Peter,Tobias
*====================================================================
* $Id: Beeper.h 3494 2011-01-31 17:36:26Z ulbrich $
*====================================================================*/
#ifndef DAC_PWM_H_INCLUDED
#define DAC_PWM_H_INCLUDED


#include <stdint.h>
class DAC {

    private:
        unsigned char ltc;
        unsigned char port;
        bool state;

    public:
        void init(unsigned char port, unsigned char baseLtc);
        void on();
        void off();
        void write(uint8_t v);
        bool getState() const { return state; };
//         void setState(bool state) { state ? on() : off(); }
};

#endif // BEEPER_H_INCLUDED
