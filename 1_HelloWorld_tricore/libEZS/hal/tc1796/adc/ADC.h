/*====================================================================
* Function: Basic abstraction for TriCore TC1796 ADC
* Description:
*
* Copyright I4Copter Project 2007-2010
* http://www4.informatik.uni-erlangen.de/Research/I4Copter
*=====================================================================
* Author: Peter Ulbrich, Adriaan Schmid, Christian Franzen
*=====================================================================
* $Id: ADC.h 3383 2010-11-09 12:06:52Z ulbrich $
*====================================================================*/
#ifndef ADC_H_INCLUDED
#define ADC_H_INCLUDED

#include <tc1796b/adc0-struct.h>
#include <tc1796b/adc1-struct.h>
#include <tc1796b/scu.h>
#include "system/SystemFrequency.h"
#include "OSAbstraction.h"

#include <machine/wdtcon.h>
#include <stdint.h>

class ADCSystem
{
    public:
        /**
         * \brief Initialise the Analog to Digital Converter
         * \param frequency Base frequency f_adc
         * \warning Note that you need to initialise the individual ADC
         *          modules additionally
         */
        static void init(unsigned long frequency)
        {
            unsigned long f_sys = SystemFrequency::Instance().sysFrequency();
            ADC0_t* pADC0 = (ADC0_t*) ADC0_BASE;

            ::unlock_wdtcon();
            pADC0->CLC.bits.DISR = 0;

            while ((pADC0->CLC.reg & 0x00000002) == 2); // wait until module is enabled

            pADC0->FDR.bits.DM = 1; // normal divider mode
            pADC0->FDR.bits.STEP = 1024 - f_sys / frequency;
            ::lock_wdtcon();
        }
};


enum adcScanmode_t
{
    ADC_SCANMODE_DISABLED = 0,
    ADC_SCANMODE_SINGLE = 1,
    ADC_SCANMODE_CONTINUOUS = 2
};

enum adcRequestNodeTOS_t
{
    ADC_HANDLED_BY_CPU = 0,
    ADC_HANDLED_BY_PCP = 1
};


#define ADC_EXT_TRIGGER_GPTA28 3


#define TD_ADC template<uint8_t _ADC_ID>
#define PL_ADC ADC<_ADC_ID>

TD_ADC class ADC
{
    public:
        /**
         * \brief Initialise the ADC module
         */
        static void init();

        /**
         * \brief Setup the ADC trigger timer
         * \param freq Trigger frequency (must be >= 229 (at f_sys=75MHz))
         *
         * This sets up the ADC trigger timer. Each ADC module has
         * only one of those, so the frequency set here will apply
         * to all channels which are timer-triggered.
         */
        static void setupTimer(unsigned long freq);

        /**
         * \brief Setup the ADC external trigger
         * \param trigger_id external trigger id (ADC_EXT_TRIGGER_*)
         *
         * This sets up the external trigger of an ADC module
         */
        static void setupExternalTrigger(uint8_t trigger_id);

        /**
         * \brief Setup an ADC unit for auto scanning
         * \param scan_mode ADC unit scan mode  (disabled, single, continuous)
         */
        static void setupScanmode(adcScanmode_t scan_mode);

        /**
         * \brief Setup an ADC service request node
         * \param node_ptr Number of the request node (0-7)
         * \param tos Type of Service (Handled by CPU or Handled by PCP)
         * \param srpn Priority (Vector) Number
         *
         * This initialises one of the four Service Request Nodes of an ADC module.
         * Service request generation is done in two steps: An ADC channel can generate
         * a request to one of the four (shared) nodes, which will then generate the
         * actual interrupt according to its settings.
         */
        static void setupRequestNode(uint8_t node_ptr, adcRequestNodeTOS_t tos, uint8_t srpn);

    private:
        static ADC0_t* const m_pADC0;
        static ADC1_t* const m_pADC1;
};


TD_ADC ADC0_t* const PL_ADC::m_pADC0 = (ADC0_t*) ADC0_BASE;
TD_ADC ADC1_t* const PL_ADC::m_pADC1 = (ADC1_t*) ADC1_BASE;

TD_ADC void PL_ADC::init()
{
    // Init the specified module
    ADCn_STAT_t* stat;

    switch (_ADC_ID)
    {
    case 0:
        TGADC0.bits.SW0GTSEL = 4; // "always enabled"
        stat = &m_pADC0->STAT;
        break;
    case 1:
        TGADC1.bits.SW0GTSEL = 4; // "always enabled"
        stat = &m_pADC1->STAT;
        break;
    default:
        osPanic();  // Abort on wrong number
    }

    // wait for the calibration to finish
    while (stat->bits.CAL == 1);
}


TD_ADC void PL_ADC::setupTimer(unsigned long freq)
{
    unsigned long f_sys = SystemFrequency::Instance().sysFrequency();
    unsigned short reload_value = f_sys / (freq * 20);

#ifdef _OS_PXROS

    ADCn_TCON_t_nonv tcon;
    tcon.bits.TRLD = reload_value;
    tcon.bits.ALB = 0; // <-- change this if you want an arbitration lock
    tcon.bits.TR = 1; // <-- "run"

    switch (_ADC_ID)
    {
    case 0:
        ADC0_TCON = tcon;
        ADC0_SCON.reg = ADCn_SCON_TRS_MASK;
        break;
    case 1:
        ADC1_TCON = tcon;
        ADC1_SCON.reg = ADCn_SCON_TRS_MASK;
        break;
    }
#endif // _OS_CIAO
}


TD_ADC void PL_ADC::setupExternalTrigger(uint8_t trigger_id)
{
    switch (_ADC_ID)
    {
    case 0:
        TGADC0.bits.ETRSEL = trigger_id;
        TGADC0.bits.EGTSEL = 4; // "always enabled"
        break;
    case 1:
        TGADC1.bits.ETRSEL = trigger_id;
        TGADC1.bits.EGTSEL = 4; // "always enabled"
        break;
    default:
        break;
    }
}


TD_ADC void PL_ADC::setupScanmode(adcScanmode_t scan_mode)
{
    switch (_ADC_ID)
    {
    case 0:
        m_pADC0->CON.bits.SCNM = static_cast<uint8_t>(scan_mode);
        break;
    case 1:
        m_pADC1->CON.bits.SCNM = static_cast<uint8_t>(scan_mode);
        break;
    default:
        return;
    }
}


TD_ADC void PL_ADC::setupRequestNode(uint8_t node_ptr, adcRequestNodeTOS_t tos, uint8_t srpn)
{
#ifdef _OS_PXROS
    ADCn_SRCm_t_nonv src;

    src.reg = 0;
    src.bits.SRPN = srpn;
    src.bits.TOS = static_cast<uint8_t>(tos);
    src.bits.SRE = 1; // enable request node

    switch (_ADC_ID)
    {
    case 0:
        switch (node_ptr)
        {
        case 0:
            m_pADC0->SRC0 = src;
            break;
        case 1:
            m_pADC0->SRC1 = src;
            break;
        case 2:
            m_pADC0->SRC2 = src;
            break;
        case 3:
            m_pADC0->SRC3 = src;
            break;
        default:
            break;
        }
        break;
    case 1:
        switch (node_ptr)
        {
        case 0:
            m_pADC1->SRC0 = src;
            break;
        case 1:
            m_pADC1->SRC1 = src;
            break;
        case 2:
            m_pADC1->SRC2 = src;
            break;
        case 3:
            m_pADC1->SRC3 = src;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
#endif // _OS_CIAO
}




#endif // ADC_H_INCLUDED
