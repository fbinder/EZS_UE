/*====================================================================
* Function: Channel abstraction for TriCore TC1796 ADC
* Description:
*
* Copyright I4Copter Project 2007-2010
* http://www4.informatik.uni-erlangen.de/Research/I4Copter
*=====================================================================
* Author: Peter Ulbrich, Adriaan Schmid, Christian Franzen
*=====================================================================
* $Id: ADCChannel.h 3834 2011-04-28 12:01:00Z ulbrich $
*====================================================================*/

#ifndef ADCCHANNEL_H_INCLUDED
#define ADCCHANNEL_H_INCLUDED

#include <tc1796b/adc0-struct.h>
#include <tc1796b/adc1-struct.h>

#include "ADC.h"

#define ADC_BY_CHA(CHA) ((CHA > 31) ? 1 : 0)

enum adcChannelScanmode_t
{
    ADC_SCANMODE_SOFTWARE = 0,
    ADC_SCANMODE_AUTOSCAN = 1
};


enum adcTriggerSource_t
{
    ADC_TRIG_SOURCE_TIMER = 1,
    ADC_TRIG_SOURCE_EXTERNAL = 2
};


#define TD_ADCCHANNEL template<uint8_t _CHANNEL_ID>
#define PL_ADCCHANNEL ADCChannel<_CHANNEL_ID>

TD_ADCCHANNEL class ADCChannel
{
public:
    ADCChannel();

    /**
     * \brief Setup the ADC channel
     * \param input_group (group_0_15 or group_16_31)
     * \param scan_mode Channel scan mode (auto-scan or software control)
     * \param resolution Sampling resolution (8, 10 or 12)
     * \param sample_time Sampling time, divider value (0-255)
     * \param v_ref Reference voltage (0-3)
     */
    void setupChannel(adcChannelScanmode_t scan_mode, uint8_t resolution, uint8_t sample_time, uint8_t v_ref);

    /**
     * \brief Enable a trigger
     * \param trigger Trigger to enable (ADC_TRIGGER_(TIMER|EXT))
     * \warning currently only timer triggering can be controlled
     *          with this function
     */
    void setTrigger(adcTriggerSource_t trigger);

    /**
     * \brief Disable a trigger
     * \param trigger Trigger to disable (ADC_TRIGGER_(TIMER|EXT))
     * \warning currently only timer triggering can be controlled
     *          with this function
     */
    void clearTrigger(adcTriggerSource_t trigger);

    /**
     * \brief Enable limit check of an ADC channel
     * \param limit_check Type of check to be performed
     */
    void enableLimitCheck(uint8_t limit_check);

    /**
     * \brief Enable service request of an ADC channel
     * \param node_ptr Node to use (0-7)
     */
    void enableServiceRequest(uint8_t node_ptr);

    /**
     * \brief Trigger ADC sampling and wait for the result
     * \return sampling result
     * \warning Note that the result is left-aligned, so it will
     *          always range from 0 to 4095, regardless of the resolution.
     */
    uint16_t sampleOnce();

    /**
     * \brief Trigger ADC sampling and do not wait for the result
     */
    void sampleNowait();

    /**
     * \brief Read result of ADC sampling
     * \return sampling result
     *
     * \warning This function will not trigger a conversion, and it will not wait
     *          until one is finished. It will ONLY return the contents of
     *          the RESULT register.
     */
    uint16_t readResult();

    /**
     * \brief Setup an ADC unit for auto scanning
     * \param scan_mode ADC unit scan mode  (disabled, single, continuous)
     */
    void setupAdcScanmode(adcScanmode_t scan_mode)
    {
        ADC<ADC_BY_CHA(_CHANNEL_ID)>::setupScanmode(scan_mode);
    }

private:
    static unsigned int m_iObjCounter;
    static ADC0_t* const m_pADC0;
    static ADC1_t* const m_pADC1;
};


TD_ADCCHANNEL unsigned int PL_ADCCHANNEL::m_iObjCounter = 0;
TD_ADCCHANNEL ADC0_t* const PL_ADCCHANNEL::m_pADC0 = (ADC0_t*) ADC0_BASE;
TD_ADCCHANNEL ADC1_t* const PL_ADCCHANNEL::m_pADC1 = (ADC1_t*) ADC1_BASE;


TD_ADCCHANNEL PL_ADCCHANNEL::ADCChannel()
{
    // Runtime check if same channel is used multiple
    PL_ADCCHANNEL::m_iObjCounter++;

    if (PL_ADCCHANNEL::m_iObjCounter > 1) osPanic();
}


TD_ADCCHANNEL void PL_ADCCHANNEL::setupChannel(adcChannelScanmode_t scan_mode, uint8_t resolution, uint8_t sample_time, uint8_t v_ref)
{
    ADCn_CHCONm_t_nonv con_temp;
    ADCn_SCN_t *scn;

    con_temp.reg = 0;
    switch (resolution)
    {
    case 8:
        con_temp.bits.RES = 2;
        break;
    case 10:
        con_temp.bits.RES = 0;
        break;
    case 12:
        con_temp.bits.RES = 1;
        break;
    default:
        return;
    }
    con_temp.bits.STC = sample_time;
    con_temp.bits.REF = v_ref;

    switch ( ADC_BY_CHA(_CHANNEL_ID) )
    {
    case 0:
        con_temp.bits.GRPS = (_CHANNEL_ID > 15 ? 1 : 0);
        m_pADC0->CHCON[_CHANNEL_ID % 16].reg = con_temp.reg;
        scn = &(m_pADC0->SCN);
        break;
    case 1:
        m_pADC1->CHCON[(_CHANNEL_ID % 32) + 4].reg = con_temp.reg;
        scn = &(m_pADC1->SCN);
        break;
    default:
        return;
    }

    // If auto-scan and ADC0 is selected, add channel to auto-scan conversation register
    if (scan_mode == ADC_SCANMODE_AUTOSCAN)
    {
        if (ADC_BY_CHA(_CHANNEL_ID) == 0)
            scn->reg |= (1 << (_CHANNEL_ID % 16));
        else
            scn->reg |= (1 << ((_CHANNEL_ID % 32) + 4) % 32);
    }
}



TD_ADCCHANNEL void PL_ADCCHANNEL::setTrigger(adcTriggerSource_t trigger)
{
    ADCn_TTC_t *ttc;
    ADCn_EXTC_t *extc;

    switch ( ADC_BY_CHA(_CHANNEL_ID) )
    {
    case 0:
        ttc = &(m_pADC0->TTC);
        extc = &(m_pADC0->EXTC);
        break;
    case 1:
        ttc = &(m_pADC1->TTC);
        extc = &(m_pADC1->EXTC);
        break;
    default:
        return;
    }

    switch (trigger)
    {
    case ADC_TRIG_SOURCE_TIMER:
        ttc->reg |= (1 << (_CHANNEL_ID % 16));
        break;
    case ADC_TRIG_SOURCE_EXTERNAL:
        extc->reg |= (1 << (_CHANNEL_ID % 16));
        break;
    default:
        return;
    }
}


TD_ADCCHANNEL void PL_ADCCHANNEL::clearTrigger(adcTriggerSource_t trigger)
{
    ADCn_TTC_t *ttc;
    ADCn_EXTC_t *extc;

    switch ( ADC_BY_CHA(_CHANNEL_ID) )
    {
    case 0:
        ttc = &(m_pADC0->TTC);
        extc = &(m_pADC0->EXTC);
        break;
    case 1:
        ttc = &(m_pADC1->TTC);
        extc = &(m_pADC1->EXTC);
        break;
    default:
        return;
    }

    switch (trigger)
    {
    case ADC_TRIG_SOURCE_TIMER:
        ttc->reg &= ~(1 << (_CHANNEL_ID % 16));
        break;
    case ADC_TRIG_SOURCE_EXTERNAL:
        extc->reg &= ~(1 << (_CHANNEL_ID % 16));;
        break;
    default:
        return;
    }

}


TD_ADCCHANNEL void PL_ADCCHANNEL::enableLimitCheck(uint8_t limit_check)
{
    ADCn_CHCONm_t *chcon;
    ADCn_CHCONm_t_nonv con_temp;

    switch ( ADC_BY_CHA(_CHANNEL_ID) )
    {
    case 0:
        chcon = &(m_pADC0->CHCON[_CHANNEL_ID % 16]);
        break;
    case 1:
        chcon = &(m_pADC1->CHCON[_CHANNEL_ID % 16]);
        break;
    default:
        return;
    }

    con_temp.reg = chcon->reg;
    con_temp.bits.LCC = limit_check;
    chcon->reg = con_temp.reg;
}


TD_ADCCHANNEL void PL_ADCCHANNEL::enableServiceRequest(uint8_t node_ptr)
{
    ADCn_CHCONm_t *chcon;
    ADCn_CHCONm_t_nonv con_temp;

    switch ( ADC_BY_CHA(_CHANNEL_ID) )
    {
    case 0:
        chcon = &(m_pADC0->CHCON[_CHANNEL_ID % 16]);
        break;
    case 1:
        chcon = &(m_pADC1->CHCON[_CHANNEL_ID % 16]);
        break;
    default:
        return;
    }

    con_temp.reg = chcon->reg;

    con_temp.bits.INP = node_ptr;
    con_temp.bits.ENCH = 1;
    con_temp.bits.LCC = 4; // generate service request on every update of RESULT

    chcon->reg = con_temp.reg;
}


TD_ADCCHANNEL uint16_t PL_ADCCHANNEL::sampleOnce()
{
    unsigned long req_mask = 0;

    switch ( ADC_BY_CHA(_CHANNEL_ID) )
    {
    case 0:
        // set software request
        req_mask = 1 << (_CHANNEL_ID % 16);
        m_pADC0->REQ0.reg |= req_mask;
        // wait until "pending" bit is cleared
        while (m_pADC0->SW0CRP.reg & req_mask);
        // and get the result
        return m_pADC0->CHSTAT[_CHANNEL_ID % 16].bits.RESULT;
        break;
    case 1:
        // set software request
        req_mask = 1 << ((_CHANNEL_ID % 32) + 4);
        m_pADC1->REQ0.reg |= req_mask;
        // wait until "pending" bit is cleared
        while (m_pADC1->SW0CRP.reg & req_mask)
            ;
        // and get the result
        return m_pADC1->CHSTAT[(_CHANNEL_ID % 32) + 4].bits.RESULT;
        break;
    default:
        return 0;
    };
}


TD_ADCCHANNEL void PL_ADCCHANNEL::sampleNowait()
{
    unsigned long req_mask = 1 << (_CHANNEL_ID % 16);

    switch ( ADC_BY_CHA(_CHANNEL_ID) )
    {
    case 0:
        m_pADC0->REQ0.reg |= req_mask;
        break;
    case 1:
        m_pADC1->REQ0.reg |= req_mask;
        break;
    default:
        break;
    }
}


TD_ADCCHANNEL uint16_t PL_ADCCHANNEL::readResult()
{
    switch ( ADC_BY_CHA(_CHANNEL_ID) )
    {
    case 0:
        return m_pADC0->CHSTAT[_CHANNEL_ID % 16].bits.RESULT;
        break;
    case 1:
        return m_pADC1->CHSTAT[(_CHANNEL_ID % 32) + 4].bits.RESULT;
        break;
    default:
        return 0;
    }
}


#endif // ADCCHANNEL_H_INCLUDED
