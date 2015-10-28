#include "SystemFrequency.h"

#include <tc1796b/scu.h>
#include <tc1796b/pmi.h>
#include <tc1796b/stm.h>

#include "SystemTimer.h"

#define QUARTZ_FREQUENCY 20000000U  /* TC1796 quartz frequency is 20 MHz */


SystemFrequency SystemFrequency::sm_instance;

SystemFrequency &SystemFrequency::Instance()
{
    return sm_instance;
}

uint32_t SystemFrequency::cpuFrequency() const
{
    return m_cpuFrequency;
}

uint32_t SystemFrequency::sysFrequency() const
{
    return m_sysFrequency;
}

uint32_t SystemFrequency::stmFrequency() const
{
    return m_stmFrequency;
}

void SystemFrequency::init()
{
    initCpuFrequency();
    initSysFrequency();
    initStmFrequency();
    SystemTimer::init();
}

void SystemFrequency::initCpuFrequency()
{
	if (PLL_CLC.bits.BYPPIN) {
		m_cpuFrequency = QUARTZ_FREQUENCY;
	} else {
		uint32_t n_div = PLL_CLC.bits.NDIV + 1;
		uint32_t k_div = PLL_CLC.bits.KDIV + 1;
		uint32_t p_div = PLL_CLC.bits.PDIV + 1;

		m_cpuFrequency = QUARTZ_FREQUENCY * n_div / (k_div * p_div);
	}
}

void SystemFrequency::initStmFrequency()
{
    uint32_t divider = STM_CLC.bits.RMC;

    if (divider == 0)
    {
        m_stmFrequency = 0;
    }
    else
    {
        m_stmFrequency = sysFrequency() / divider;
    }
}

void SystemFrequency::initSysFrequency()
{
    if (PLL_CLC.bits.SYSFS == 0)
    {
        m_sysFrequency = cpuFrequency() / 2;
    }
    else
    {
        m_sysFrequency = cpuFrequency();
    }
}

