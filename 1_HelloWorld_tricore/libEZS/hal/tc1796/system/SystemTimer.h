/*====================================================================
* Function: Basic abstraction for TriCore TC1796 System Timer
* Description:
*
* Copyright I4Copter Project 2007-2010
* http://www4.informatik.uni-erlangen.de/Research/I4Copter
*=====================================================================
* Author: Peter Ulbrich, Florian Franzmann, Christian Franzen
*=====================================================================
* $Id: SystemTimer.h 2985 2010-07-16 14:46:02Z sijesche $
*====================================================================*/
#ifndef SYSTEMTIMER_H
#define SYSTEMTIMER_H

#include "SystemFrequency.h"
#include <math.h>

#include <tc1796b/scu.h>
#include <tc1796b/pmi.h>
#include <tc1796b/stm.h>

class SystemTimer {
    private:
        SystemTimer();
        static float usPerTick_timer1;
        static float ticksPerMs_timer1;
        static float msPerTick_timer4;
        static float ticksPerMs_timer4;
        static double ticksPerNs_timerFull;
        static double nsPerTick_timerFull;
    public:
        static void init();
        static inline uint32_t frequency();
        static inline uint32_t timer0();
        static inline uint32_t timer1();
        static inline uint32_t timer2();
        static inline uint32_t timer3();
        static inline uint32_t timer4();
        static inline uint32_t timer5();
        static inline uint32_t timer6();
        static inline uint32_t timerCapture();
        static uint64_t timerFull();


        static inline uint32_t milliseconds();
        static inline uint32_t ticksToMicroseconds(uint32_t ticks);
        static uint32_t millisecondsToTicks(uint16_t ms);
        static uint32_t millisecondsToTicks_timer1(uint16_t ms);
        static uint32_t ticksToMicroseconds_timer1(uint32_t ticks);
        static uint64_t nanosecondsToTicks_timerFull(uint64_t ns);
        static uint64_t ticksToNanoseconds_timerFull(uint64_t ticks);
};


inline uint32_t SystemTimer::frequency()
{
    return SystemFrequency::Instance().stmFrequency();
}

inline uint32_t SystemTimer::timer0()
{
    return STM_TIM0.reg;
}

inline uint32_t SystemTimer::timer1()
{
    return STM_TIM1.reg;
}

inline uint32_t SystemTimer::timer2()
{
    return STM_TIM2.reg;
}

inline uint32_t SystemTimer::timer3()
{
    return STM_TIM3.reg;
}

inline uint32_t SystemTimer::timer4()
{
    return STM_TIM4.reg;
}

inline uint32_t SystemTimer::timer5()
{
    return STM_TIM5.reg;
}

inline uint32_t SystemTimer::timer6()
{
    return STM_TIM6.reg;
}

inline uint32_t SystemTimer::timerCapture()
{
    return STM_CAP.reg;
}


/** Use this function with timer ticks from timer4 only **/
inline uint32_t SystemTimer::milliseconds(void)
{
    return static_cast<uint32_t>(timer4() * (SystemTimer::msPerTick_timer4));
}

/** Use this function with timer ticks from timer4 only **/
inline uint32_t SystemTimer::millisecondsToTicks(uint16_t ms) {
    return static_cast<uint32_t>(ceilf(static_cast<float>(ms) * SystemTimer::ticksPerMs_timer4));
}

/** us Resolution: use this function with timer1 **/
inline uint32_t SystemTimer::millisecondsToTicks_timer1(uint16_t ms) {
    return static_cast<uint32_t>(ceilf(static_cast<float>(ms) * SystemTimer::ticksPerMs_timer1));
}

inline uint32_t SystemTimer::ticksToMicroseconds_timer1(uint32_t ticks)
{
    return  static_cast<uint32_t>( ticks *  SystemTimer::usPerTick_timer1);
}

inline uint64_t SystemTimer::nanosecondsToTicks_timerFull(uint64_t ns)
{
    return static_cast<uint64_t>(ceilf(static_cast<float>(ns)
     * SystemTimer::ticksPerNs_timerFull));
}

inline uint64_t SystemTimer::ticksToNanoseconds_timerFull(uint64_t ticks)
{
    return static_cast<uint64_t>(ticks * SystemTimer::nsPerTick_timerFull);
}

#endif // SYSTEMTIMER_H
