/*====================================================================
* Function: Basic abstraction for TriCore TC1796 DCM
* Description:
*
* Copyright I4Copter Project 2007-2010
* http://www4.informatik.uni-erlangen.de/Research/I4Copter
*=====================================================================
* Author: Peter Ulbrich, Adriaan Schmid, Christian Franzen
*=====================================================================
* $Id: DCM.h 3186 2010-09-16 10:40:01Z mike $
*====================================================================*/
#ifndef DCM_H_INCLUDED
#define DCM_H_INCLUDED

#include <tc1796b/gpta0-struct.h>
#include <tc1796b/gpta1-struct.h>

#include <stdint.h>

#define DCM_GET_FROM_FPC(x) ((x == 0) ? (0) : \
                             (x == 2) ? (1) : \
                             (x == 3) ? (2) : \
                             (x == 5) ? (3) : 42)


/* DCM timer reset mode */
enum dcmEdge_t
{
    DCM_EDGE_NONE = 0, /* no edge */
    DCM_EDGE_RISE = 1, /* rising edge */
    DCM_EDGE_FALL = 2, /* falling edge */
    DCM_EDGE_BOTH = 3  /* both edges */
};


/* DCM timers */
enum dcmTimer_t
{
    DCM_FALLING_EDGE_TIMER = 0, /* timer triggered on falling edge */
    DCM_RISING_EDGE_TIMER  = 1  /* timer triggered on rising edge */
};

/* DCM interrupts */
enum dcmInterrupt_t
{
    DCM_IRQ_NONE = 0, /* do not trigger any interrupts */
    DCM_IRQ_RISE = 1, /* trigger an interrupt on a rising edge */
    DCM_IRQ_FALL = 2, /* trigger an interrupt on a falling edge */
    DCM_IRQ_BOTH = 3, /* trigger an interrupt on both edges */
    DCM_IRQ_COMP = 4  /* trigger an interrupt when the timer hit the limit
	                     set in the capcom register */
};

/* DCM capture */
enum dcmCapture_t
{
    DCM_CAPTURE_FALL = 0, /* capture value on falling edge */
    DCM_CAPTURE_RISE  = 1  /* capture value on rising edge */
};

/* DCM capture on opposite edge mode */
enum dcmCapcom_t
{
    DCM_CAPCOM_OFF = 0,
    DCM_CAPCOM_ON = 1
};


/* Struct for storing register pointers of the DCM's */
struct dcmRegPointer_t
{
    GPTAn_DCMCTRm_t *ctr; /* control */
    GPTAn_DCMTIMm_t *tim; /* timer */
    GPTAn_DCMCAVm_t *cav; /* caputure */
    GPTAn_DCMCOVm_t *cov; /* caputure/compare */
    GPTAn_SRSC0_t   *srsc0;
    GPTAn_SRSS0_t   *srss0;
};


#define TD_DCM template<uint8_t _GPTA_ID>
#define PL_DCM DCM<_GPTA_ID>

TD_DCM class DCM
{
    public:
        /**
         * \brief Initialize the specified Duty Cycle Measurement Unit
         * \param dcm_number The DCM instance to use (0-3)
         */
        static void init(uint8_t dcm_number);

        /**
         * \brief Set the edge(s) to reset the timer on.
         * \param dcm_number The DCM instance to use (0-3)
         * \param edge The edge(s) to reset the timer on (if DCM_EDGE_NONE is selected
         *        the timer will "reset" on a overflow)
         * \return true on success, false else
         */
        static bool setTimerResetMode(uint8_t dcm_number, dcmEdge_t edge);

        /**
         * \brief Set capture mode for Duty Cycle Measurement Unit
         * \param dcm_number The DCM instance to use (0-3)
         * \param trigger The trigger for capture event (rise/fall)
         * \param capcom Capture opposite event to capture/compare register (on/off)
         * \return true on success, false else
         */
        static bool setCaptureMode(uint8_t dcm_number, dcmCapture_t trigger, dcmCapcom_t capcom);

        /**
         * \brief Check if a falling edge has been detected.
         * \param dcm_number The DCM instance to use (0-3)
         * \return true if a falling edge has been detected, false else
         */
        static bool hasFallingEdge(uint8_t dcm_number);

        /**
         * \brief Check if a rising edge has been detected.
         * \param dcm_number The DCM instance to use (0-3)
         * \return true if a rising edge has been detected, false else
         */
        static bool hasRisingEdge(uint8_t dcm_number);

        /**
         * \brief Get the current timer value.
         * \param dcm_number The DCM instance to use (0-3)
         * \param timer The timer to get the value from (rising / falling edge)
         * \return Current 24-bit timer value (the first 8 bit are always 0)
         */
        static uint32_t getTimerValue(uint8_t dcm_number, dcmTimer_t timer);

        /**
         * \brief Enable interrupts of the DCM.
         * \param dcm_number The DCM instance to use (0-3)
         * \param interrupt The interrupt to enable
         * \return true on success, false else
         * \warning You need to enable the appropriate IS node yourself (by calling
         *          IS::enableNode()).
         */
        static bool enableInterrupt(uint8_t dcm_number, dcmInterrupt_t interrupt);

        /**
         * \brief Set the hardware output pulse mode.
         * \param dcm_number The DCM instance to use (0-3)
         * \param edge The edge(s) to trigger the output pulse on
         * \return true on success, false else
         */
        static bool setHwPulseMode(uint8_t dcm_number, dcmEdge_t edge);

        /**
         * \brief Generate a single output pulse.
         * \param dcm_number The DCM instance to use (0-3)
         * \return true on success, false else
         */
        static bool genOutputPulse(uint8_t dcm_number);

        /**
         * \brief Set the value of the capture/compare register.
         * \param dcm_number The DCM instance to use (0-3)
         * \param value The new 24-bit value to assign to the register
         * \return true on success, false else
         */
        static bool setCapcomRegister(uint8_t dcm_number, uint32_t value);

    private:
        static const dcmRegPointer_t pDcmRegs[4];
};

TD_DCM void PL_DCM::init(uint8_t dcm_number)
{
    if (3 < dcm_number)
    {
//        osPanic();
    }

    if ( _GPTA_ID > 1 ) {
//        osPanic();
    }

    const dcmRegPointer_t* dcm = &pDcmRegs[dcm_number];

    dcm->ctr->reg = (unsigned long)0;
    dcm->tim->reg = (unsigned long)0;
    dcm->cav->reg = (unsigned long)0;
    dcm->cov->reg = (unsigned long)0;
}


TD_DCM bool PL_DCM::setTimerResetMode(uint8_t dcm_number, dcmEdge_t edge)
{
    if ((3 < dcm_number) /*|| (3 < edge)*/)
        return false;

    const dcmRegPointer_t* dcm = &pDcmRegs[dcm_number];

    if ((DCM_EDGE_RISE == edge) || (DCM_EDGE_BOTH == edge))
        dcm->ctr->bits.RZE = 1;
    else
        dcm->ctr->bits.RZE = 0;

    if ((DCM_EDGE_FALL == edge) || (DCM_EDGE_BOTH == edge))
        dcm->ctr->bits.FZE = 1;
    else
        dcm->ctr->bits.FZE = 0;

    return true;
}


TD_DCM bool PL_DCM::setCaptureMode(uint8_t dcm_number, dcmCapture_t trigger, dcmCapcom_t capcom)
{
    if ((3 < dcm_number) /*|| (1 < trigger)*/)
        return false;

    const dcmRegPointer_t* dcm = &pDcmRegs[dcm_number];

    if (DCM_CAPTURE_RISE == trigger)
        dcm->ctr->bits.RCA = 1;
    else if (DCM_CAPTURE_FALL == trigger)
        dcm->ctr->bits.RCA = 0;

    if (DCM_CAPCOM_ON == capcom)
        dcm->ctr->bits.OCA = 1;
    else if (DCM_CAPCOM_OFF == capcom)
        dcm->ctr->bits.OCA = 0;

    return true;
}


TD_DCM bool PL_DCM::hasFallingEdge(uint8_t dcm_number)
{
    if (3 < dcm_number) return false;

    int ret = -1;
    const dcmRegPointer_t* dcm = &pDcmRegs[dcm_number];

    switch (dcm_number)
    {
    case 0:
        ret = dcm->srss0->bits.DCM00F;
        if (1 == ret)
            dcm->srsc0->bits.DCM00F = 1;
        break;
    case 1:
        ret = dcm->srss0->bits.DCM01F;
        if (1 == ret)
            dcm->srsc0->bits.DCM01F = 1;
        break;
    case 2:
        ret = dcm->srss0->bits.DCM02F;
        if (1 == ret)
            dcm->srsc0->bits.DCM02F = 1;
        break;
    case 3:
        ret = dcm->srss0->bits.DCM03F;
        if (1 == ret)
            dcm->srsc0->bits.DCM03F = 1;
        break;
    }

    return (ret == 1) ? true : false;
}


TD_DCM bool PL_DCM::hasRisingEdge(uint8_t dcm_number)
{
    if (3 < dcm_number) return false;

    int ret = -1;
    const dcmRegPointer_t* dcm = &pDcmRegs[dcm_number];

    switch (dcm_number)
    {
    case 0:
        ret = dcm->srss0->bits.DCM00R;
        if (1 == ret)
            dcm->srsc0->bits.DCM00R = 1;
        break;
    case 1:
        ret = dcm->srss0->bits.DCM01R;
        if (1 == ret)
            dcm->srsc0->bits.DCM01R = 1;
        break;
    case 2:
        ret = dcm->srss0->bits.DCM02R;
        if (1 == ret)
            dcm->srsc0->bits.DCM02R = 1;
        break;
    case 3:
        ret = dcm->srss0->bits.DCM03R;
        if (1 == ret)
            dcm->srsc0->bits.DCM03R = 1;
        break;
    }

    return (ret == 1) ? true : false;
}


TD_DCM uint32_t PL_DCM::getTimerValue(uint8_t dcm_number, dcmTimer_t timer)
{
    if ((3 < dcm_number) /*|| (1 < timer)*/)
        return 0; /* there is no reasonable error indicator */

    unsigned char fe_timer;
    const dcmRegPointer_t* dcm = &pDcmRegs[dcm_number];

    if (DCM_FALLING_EDGE_TIMER == timer)
    {
        fe_timer = 0;
    }
    else
    {
        fe_timer = 1;
    }

    if (fe_timer == dcm->ctr->bits.RCA) /* CAV contains falling edge timer */
        return dcm->cav->bits.CAV;
    else if (1 == dcm->ctr->bits.OCA) /* COV contains opposite */
        return dcm->cov->bits.COV;

    /* if OCA == 0, COV does not contain any timer value */
    return 0;
}


TD_DCM bool PL_DCM::enableInterrupt(uint8_t dcm_number, dcmInterrupt_t interrupt)
{
    if ((3 < dcm_number) /*|| (4 < interrupt)*/) return false;

    const dcmRegPointer_t* dcm = &pDcmRegs[dcm_number];

    if (DCM_IRQ_BOTH == interrupt)
        dcm->ctr->bits.OCA = 1;
    else if (DCM_IRQ_RISE == interrupt)
        dcm->ctr->bits.RCA = 1;
    else if (DCM_IRQ_FALL == interrupt)
        dcm->ctr->bits.RCA = 0;

    if ((DCM_IRQ_RISE == interrupt) || (DCM_IRQ_BOTH == interrupt))
        dcm->ctr->bits.RRE = 1;
    else
        dcm->ctr->bits.RRE = 0;

    if ((DCM_IRQ_FALL == interrupt) || (DCM_IRQ_BOTH == interrupt))
        dcm->ctr->bits.FRE = 1;
    else
        dcm->ctr->bits.FRE = 0;

    if (DCM_IRQ_COMP == interrupt)
        dcm->ctr->bits.CRE = 1;
    else
        dcm->ctr->bits.CRE = 0;

    return true;
}


TD_DCM bool PL_DCM::setHwPulseMode(uint8_t dcm_number, dcmEdge_t edge)
{
    if ((3 < dcm_number) /*|| (3 < edge)*/) return false;

    const dcmRegPointer_t* dcm = &pDcmRegs[dcm_number];

    if ((DCM_EDGE_RISE == edge) || (DCM_EDGE_BOTH == edge))
        dcm->ctr->bits.RCK = 1;
    else
        dcm->ctr->bits.RCK = 0;

    if ((DCM_EDGE_FALL == edge) || (DCM_EDGE_BOTH == edge))
        dcm->ctr->bits.FCK = 1;
    else
        dcm->ctr->bits.FCK = 0;

    return true;
}


TD_DCM bool PL_DCM::genOutputPulse(uint8_t dcm_number)
{
    if (3 < dcm_number) return false;

    (&pDcmRegs[dcm_number])->ctr->bits.QCK = 1;

    return true;
}


TD_DCM bool PL_DCM::setCapcomRegister(uint8_t dcm_number, uint32_t value)
{
    if (3 < dcm_number) return false;

    (&pDcmRegs[dcm_number])->cov->bits.COV = value;

    return true;
}

#endif // DCM_H_INCLUDED
