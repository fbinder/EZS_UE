#include "SystemTimer.h"
#include "SystemFrequency.h"

float SystemTimer::usPerTick_timer1 = 0;
float SystemTimer::ticksPerMs_timer1 = 0;
float SystemTimer::msPerTick_timer4 = 0;
float SystemTimer::ticksPerMs_timer4 = 0;
double SystemTimer::ticksPerNs_timerFull = 0;
double SystemTimer::nsPerTick_timerFull = 0;

SystemTimer::SystemTimer()
{
}

void SystemTimer::init()
{
    uint32_t F = frequency();
    SystemTimer::msPerTick_timer4 = (32768000.0f / F);
    SystemTimer::ticksPerMs_timer4 = ( F / 32768000.0f);

    SystemTimer::ticksPerMs_timer1 = (F / 16000.0f);
    SystemTimer::usPerTick_timer1 = (16000000.0f / F); // 16/F [s] * 1000000 [us/s] = 0,213 us per Tick

    SystemTimer::nsPerTick_timerFull = (static_cast<double>(F) / 1000000000.0);
    SystemTimer::ticksPerNs_timerFull = (1000000000.0 / static_cast<double>(F));
 }

uint64_t SystemTimer::timerFull()
{
    uint64_t retUpper = 0;
    uint64_t retLower = 0;
    uint64_t retVal = 0;


    retLower = timer0();
    retUpper = timerCapture();
    retUpper = (retUpper << 32);
    retVal = retLower | retUpper;

    return retVal;
}

