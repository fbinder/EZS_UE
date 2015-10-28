/*====================================================================
* Function: Basic abstraction for TriCore TC1796 LTC
* Description:
*
* Copyright I4Copter Project 2007-2010
* http://www4.informatik.uni-erlangen.de/Research/I4Copter
*=====================================================================
* Author: Peter Ulbrich, Adriaan Schmid, Christian Franzen
*=====================================================================
* $Id: LTC.h 2969 2010-07-13 13:12:46Z ulbrich $
*====================================================================*/
#ifndef LTC_H_INCLUDED
#define LTC_H_INCLUDED


enum ltcSoMode_t
{
    LTC_SO_NONE     = 0,
    LTC_SO_HI       = 1,
    LTC_SO_LO       = 2,
    LTC_SO_BOTH     = 3
};

enum ltcComparatorAction_t
{
    LTC_COMPARATOR_ACTION_HOLD      = 0,
    LTC_COMPARATOR_ACTION_TOGGLE    = 1,
    LTC_COMPARATOR_ACTION_RESET     = 2,
    LTC_COMPARATOR_ACTION_SET       = 3
};

enum ltcTriggerEvent_t
{
    LTC_EVENT_LOCAL     = 0,
    LTC_EVENT_ALL       = 4
};


#define TD_LTC template<uint8_t _GPTA_ID>
#define PL_LTC LTC<_GPTA_ID>
#include "OSAbstraction.h"
TD_LTC class LTC
{
    public:
        /**
         * Setup an LTC as reset timer
         * \param ltc_number The LTC to use (0-63)
         */
        static void setupResetTimer(uint8_t ltc_number);

        /**
         * Setup an LTC as comparator
         * \param ltc_number The LTC to use (0-63)
         * \param compare_value Value we compare against
         * \param so_mode defines when the cell should be enabled (LTC_SO_(NONE|HI|LO|BOTH))
         */
        static void setupComparator(uint8_t ltc_number, uint16_t compare_value, ltcSoMode_t so_mode);

        /**
         * Set the action of a compare LTC
         * \param ltc_number The LTC to use (0-63)
         * \param action Defines the action to invoke upon an event (LTC_COMPARATOR_ACTION_(HOLD|SET|RESET|TOGGLE))
         * \param on_event Defines which events trigger an action (LTC_EVENT_(LOCAL|ALL))
         */
        static void setComparatorAction(uint8_t ltc_number, ltcComparatorAction_t action, ltcTriggerEvent_t event);

        /**
         * Setup an LTC as capture
         * \param ltc_number The LTC to use (0-63)
         * \param so_mode defines when the cell should be enabled (LTC_SO_(NONE|HI|LO|BOTH))
         */
        static void setupCapture(uint8_t ltc_number, ltcSoMode_t so_mode);

        /**
         * Setup an LTC as reset timer in edge mode
         * \param ltc_number The LTC to use (0-63)
         * \param so_mode defines when the cell should be enabled (LTC_SO_(NONE|HI|LO|BOTH))
         */
        static void setupResetTimerEdge(uint8_t ltc_number, ltcSoMode_t so_mode);

        /**
         * Enable an LTC's service requeset
         * \param ltc_number The LTC to use (0-63)
         * \warning Note that you must also configure the corresponding request node
         *          for your interrupt to be generated. Use is_enable_node() to do this.
         */
        static void enableServiceRequest(uint8_t ltc_number);

        /**
         * Disable an LTC's service request
         * \param ltc_number The LTC to use (0-63)
         * \warning This will only disable the service request in the LTC, not the
         *          request node, as some other LTC might be using it.
         */
        static void disableServiceRequest(uint8_t ltc_number);

        /**
         * Set the X value of an LTC
         * \param ltc_number The LTC to use (0-63)
         * \param x New X value
         * \warning If you use this in an application while the LTC is running,
         *          you might want to consider a coherent update (look it
         *          up in the manual).
         */
        static void setX(uint8_t ltc_number, uint16_t x);

        /**
         * Get the X value of an LTC
         * \param gpta_number The GPTA to use (0 or 1)
         * \param ltc_number The LTC to use (0-63)
         * \return x X value
         */
        static uint16_t getX(uint8_t ltc_number);

        /**
         * Enable/disable one short mode of an LTC
         * \param ltc_number The LTC to use (0-63)
         * \param enabled Determin if the One Shot Mode should be used (true) or not (false)
         */
        static void enableOneShotMode(uint8_t ltc_number, bool enabled);

        /**
         * Enable/disable an LTC cell running in one shot mode
         * \param ltc_number The LTC to use (0-63)
         */
        static void enableCell(uint8_t ltc_number);

    private:
        static GPTA0_t* const m_pGPTA0;
        static GPTA1_t* const m_pGPTA1;
};


TD_LTC GPTA0_t* const PL_LTC::m_pGPTA0 = (GPTA0_t *) GPTA0_BASE;
TD_LTC GPTA1_t* const PL_LTC::m_pGPTA1 = (GPTA1_t *) GPTA1_BASE;


TD_LTC void PL_LTC::setupResetTimer(uint8_t ltc_number)
{
    GPTAn_LTCCTRm_t_nonv ltc_temp;
    GPTA_LTC_t *ltc;

    switch (_GPTA_ID)
    {
        case 0:
            ltc = &(m_pGPTA0->LTC[ltc_number]);
            break;
        case 1:
            ltc = &(m_pGPTA1->LTC[ltc_number]);
            break;
        default:
            osPanic();
    }

    ltc_temp.reg = 0;
    ltc_temp.bits.MOD = 3; // reset timer
    ltc_temp.bits.ILM = 1; // level mode (to use it with a clock bus input)
    ltc_temp.bits.FED = 0;
    ltc_temp.bits.RED = 0;

    ltc->CTR.reg = ltc_temp.reg;
    ltc->XR.bits.X = 0;
}


TD_LTC void PL_LTC::setupComparator(uint8_t ltc_number, uint16_t compare_value, ltcSoMode_t so_mode)
{
    GPTAn_LTCCTRm_t_nonv ltc_temp;
    GPTA_LTC_t *ltc;

    switch (_GPTA_ID)
    {
        case 0:
            ltc = &(m_pGPTA0->LTC[ltc_number]);
            break;
        case 1:
            ltc = &(m_pGPTA1->LTC[ltc_number]);
            break;
        default:
            osPanic();
    }

    ltc_temp.reg = 0;
    ltc_temp.bits.MOD = 1; // compare mode
    ltc_temp.bits.FED = (so_mode & LTC_SO_HI) ? 1 : 0; // SOH
    ltc_temp.bits.RED = (so_mode & LTC_SO_LO) ? 1 : 0; // SOL

    // period counter
    ltc_temp.reg = 0;
    ltc_temp.bits.MOD = 1; // compare mode
    ltc_temp.bits.FED = 1; // SOH
    ltc_temp.bits.RED = 1; // SOL

    ltc->CTR.reg = ltc_temp.reg;
    ltc->XR.bits.X = compare_value;
}


TD_LTC void PL_LTC::setComparatorAction(uint8_t ltc_number, ltcComparatorAction_t action, ltcTriggerEvent_t event)
{
    GPTAn_LTCCTRm_t_nonv ltc_temp;
    GPTA_LTC_t *ltc;

    switch (_GPTA_ID)
    {
        case 0:
            ltc = &(m_pGPTA0->LTC[ltc_number]);
            break;
        case 1:
            ltc = &(m_pGPTA1->LTC[ltc_number]);
            break;
        default:
            osPanic();
    }

    ltc_temp.reg = ltc->CTR.reg;
    ltc_temp.bits.OCM = action | event;
    ltc->CTR.reg = ltc_temp.reg;
}


TD_LTC void PL_LTC::setupCapture(uint8_t ltc_number, ltcSoMode_t so_mode)
{
    GPTAn_LTCCTRm_t_nonv ltc_temp;
    GPTA_LTC_t *ltc;

    switch (_GPTA_ID)
    {
        case 0:
            ltc = &(m_pGPTA0->LTC[ltc_number]);
            break;
        case 1:
            ltc = &(m_pGPTA1->LTC[ltc_number]);
            break;
        default:
            osPanic();
    }

    ltc_temp.reg = 0;
    ltc_temp.bits.MOD = 0;
    ltc_temp.bits.FED = (so_mode & LTC_SO_HI) ? 1 : 0;
    ltc_temp.bits.RED = (so_mode & LTC_SO_LO) ? 1 : 0;

    ltc->CTR.reg = ltc_temp.reg;

}


TD_LTC void PL_LTC::setupResetTimerEdge(uint8_t ltc_number, ltcSoMode_t so_mode)
{
    GPTAn_LTCCTRm_t_nonv ltc_temp;
    GPTA_LTC_t *ltc;

    switch (_GPTA_ID)
    {
        case 0:
            ltc = &(m_pGPTA0->LTC[ltc_number]);
            break;
        case 1:
            ltc = &(m_pGPTA1->LTC[ltc_number]);
            break;
        default:
            osPanic();
    }

    ltc_temp.reg = 0;
    ltc_temp.bits.MOD = 3;
    ltc_temp.bits.ILM = 0;
    ltc_temp.bits.FED = (so_mode & LTC_SO_HI) ? 1 : 0;
    ltc_temp.bits.RED = (so_mode & LTC_SO_LO) ? 1 : 0;

    ltc->CTR.reg = ltc_temp.reg;
    ltc->XR.bits.X = 0;
}


TD_LTC void PL_LTC::enableServiceRequest(uint8_t ltc_number)
{
    switch (_GPTA_ID)
    {
        case 0:
            m_pGPTA0->LTC[ltc_number].CTR.bits.REN = 1;
            break;
        case 1:
            m_pGPTA1->LTC[ltc_number].CTR.bits.REN = 1;
            break;
        default:
            osPanic();
    }
}


TD_LTC void PL_LTC::disableServiceRequest(uint8_t ltc_number)
{
    GPTAn_LTCCTRm_t_nonv ltc_temp;
    GPTA_LTC_t *ltc;

    switch (_GPTA_ID)
    {
        case 0:
            ltc = &(m_pGPTA0->LTC[ltc_number]);
            break;
        case 1:
            ltc = &(m_pGPTA1->LTC[ltc_number]);
            break;
        default:
            osPanic();
    }

    ltc_temp.reg = ltc->CTR.reg;
    ltc_temp.bits.REN = 0;
    ltc->CTR.reg = ltc_temp.reg;
}


TD_LTC void PL_LTC::setX(uint8_t ltc_number, uint16_t x)
{
    switch (_GPTA_ID)
    {
        case 0:
            m_pGPTA0->LTC[ltc_number].XR.bits.X = x;
            break;
        case 1:
            m_pGPTA1->LTC[ltc_number].XR.bits.X = x;
            break;
        default:
            osPanic();
    }
}


TD_LTC uint16_t PL_LTC::getX(uint8_t ltc_number)
{
    switch (_GPTA_ID)
    {
        case 0:
            return m_pGPTA0->LTC[ltc_number].XR.bits.X;
        case 1:
            return m_pGPTA1->LTC[ltc_number].XR.bits.X;
        default:
            osPanic();
            return 0;
    }
}


TD_LTC void PL_LTC::enableOneShotMode(uint8_t ltc_number, bool enabled)
{
    switch (_GPTA_ID)
    {
        case 0:
            m_pGPTA0->LTC[ltc_number].CTR.bits.OSM = (enabled) ? 1 : 0;
            break;
        case 1:
            m_pGPTA1->LTC[ltc_number].CTR.bits.OSM = (enabled) ? 1 : 0;
            break;
        default:
            osPanic();
    }
}


TD_LTC void PL_LTC::enableCell(uint8_t ltc_number)
{
    switch (_GPTA_ID)
    {
        case 0:
            //g_GPTA0->LTC[ltc_number].CTR.bits.CUDCLR = 1;
            m_pGPTA0->LTC[ltc_number].CTR.bits.CUDCLR = 0;
            break;
        case 1:
            //g_GPTA1->LTC[ltc_number].CTR.bits.CUDCLR = 1;
            m_pGPTA1->LTC[ltc_number].CTR.bits.CUDCLR = 0;
            break;
        default:
            osPanic();
    }
}

#endif // LTC_H_INCLUDED
