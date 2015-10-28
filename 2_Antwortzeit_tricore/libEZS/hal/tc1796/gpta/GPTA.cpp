
#include "GPTA.h"
#include "../system/SystemFrequency.h"
#include <machine/wdtcon.h>
#include <machine/intrinsics.h>

#include <tc1796b/gpta0.h>

uint32_t GPTA::m_iFreq = 0;


void GPTA::init(uint32_t frequency)
{
    uint32_t f_sys = SystemFrequency::Instance().sysFrequency();
    m_iFreq = frequency;

    /*
     * all this isync'ing and dsync'ing is most
     * probably too much, but at least one of those
     * is needed to make it run from flash, and i
     * don't have the nerve to test which one...
     * all works fine without them when running from
     * external ram...
     */

    unlock_wdtcon();
    _isync();
    _dsync();
    GPTA0_CLC.bits.DISR = 0;
    _isync();
    _dsync();
    GPTA0_FDR.bits.DM = 1; // fdr normal mode
    _isync();
    _dsync();
    GPTA0_FDR.bits.STEP = 1024 - f_sys / frequency;
    _isync();
    _dsync();
    lock_wdtcon();
}


bool GPTA::initModule(uint8_t gpta_number)
{
    if (gpta_number != 0 && gpta_number != 1)
        return false;

    /*
     * enable GPTA
     */
    if (gpta_number == 0) {
        GPTA0_EDCTR.bits.G0EN = 1;
    } else {
        GPTA0_EDCTR.bits.G1EN = 1;
    }

    return true;
}


uint32_t GPTA::getFrequency()
{
    return m_iFreq;
}


float GPTA::getTimePerTick()
{
    // Get sys frequency
    uint32_t f_sys = SystemFrequency::Instance().sysFrequency();

    // Determin time per tick
    return ((float)(1024 - GPTA0_FDR.bits.STEP) / f_sys);
}

