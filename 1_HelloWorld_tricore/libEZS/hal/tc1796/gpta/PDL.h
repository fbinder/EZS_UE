/*====================================================================
* Function: Basic abstraction for TriCore TC1796 PDL
* Description:
*
* Copyright I4Copter Project 2007-2010
* http://www4.informatik.uni-erlangen.de/Research/I4Copter
*=====================================================================
* Author: Peter Ulbrich, Adriaan Schmid, Christian Franzen
*=====================================================================
* $Id: PDL.h 3150 2010-09-07 16:00:22Z sijesche $
*====================================================================*/
#ifndef PDL_H_INCLUDED
#define PDL_H_INCLUDED

#include "OSAbstraction.h"
#include <stdint.h>
#include <tc1796b/gpta0-struct.h>
#include <tc1796b/gpta1-struct.h>

/* get the PDL number from the FPC number */
#define PDL_GET_FROM_FPC(x) ((int)(x/3))


/* output signal source selection */
enum pdlSignalSource_t
{
	PDL_FEED_THROUGH = 0, /* DCM cell input is driven by fed-thru FPC output
	                         PDL0: FPC0 -> DCM0
	                         PDL1: FPC3 -> DCM2 */
	PDL_PHASE_DISC   = 1  /* DCM cell input is provided with PDL pulses
	                         PDL0: PDL0 -> DCM0
	                         PDL1: PDL1 -> DCM2 */
};

/* 3-sensor mode selection */
enum pdlSensorMode_t
{
	PDL_2_SENSOR = 0, /* 2-sensor mode
	                     PDL0: FPC2 -> DCM1
	                     PDL1: FPC5 -> DCM3 */
	PDL_3_SENSOR = 1  /* 3-sensor mode
	                     PDL0: PDL0 -> DCM1
	                     PDL1: PDL1 -> DCM3 */
};

#define TD_PDL template<uint8_t _GPTA_ID>
#define PL_PDL PDL<_GPTA_ID>

TD_PDL class PDL
{
    public:
        /**
         * \brief Initialize the specified Phase Discrimination Logic
         * \param pdl_number The PDL instance to use (0-1)
         */
        static void init(uint8_t pdl_number);

        /**
         * \brief Select the signal input source of the PDL
         * \param source The new source to use
         */
        static void selectSignalSource(uint8_t pdl_number, pdlSignalSource_t source);

        /**
         * \brief Select the sensor mode of the PDL
         * \param mode The new mode to use
         */
        static void selectSensorMode(uint8_t pdl_number, pdlSensorMode_t mode);

        /**
         * \brief Set the error flag of the PDL
         * \param flag Determin if the flag should be set (true) or unset (false)
         */
        static void setErrorFlag(uint8_t pdl_number, bool flag = true);

        /**
         * \brief Get the error flag of the PDL
         * \return The current value of the flag
         */
        static bool getErrorFlag(uint8_t pdl_number);

    private:
        static GPTAn_PDLCTR_t* const m_pControlReg[2];
};

TD_PDL void PL_PDL::init(uint8_t pdl_number)
{
    // Break on wrong GPTA or PDL number
	if ((1 < _GPTA_ID) || (1 < pdl_number))
    {
        osPanic();
    }

    // Initialize control register
	m_pControlReg[pdl_number]->reg = (unsigned long)0;
}


TD_PDL void PL_PDL::selectSignalSource(uint8_t pdl_number, pdlSignalSource_t source)
{
	if (source == PDL_FEED_THROUGH)
	{
		m_pControlReg[pdl_number]->reg &= ~(1 << (4 * pdl_number));
	}
	else
	{
		m_pControlReg[pdl_number]->reg |= 1 << (4 * pdl_number);
	}
}


TD_PDL void PL_PDL::selectSensorMode(uint8_t pdl_number, pdlSensorMode_t mode)
{
	if (mode == PDL_2_SENSOR)
	{
		m_pControlReg[pdl_number]->reg &= ~(1 << (4 * pdl_number + 1));
	}
	else
	{
		m_pControlReg[pdl_number]->reg |= 1 << (4 * pdl_number + 1);
	}
}


TD_PDL void PL_PDL::setErrorFlag(uint8_t pdl_number, bool flag)
{
	if (!flag)
	{
		m_pControlReg[pdl_number]->reg &= ~(1 << (4 * pdl_number + 2));
	}
	else
	{
		m_pControlReg[pdl_number]->reg |= 1 << (4 * pdl_number + 2);
	}
}


TD_PDL bool PL_PDL::getErrorFlag(uint8_t pdl_number)
{
	if ( (m_pControlReg[pdl_number]->reg & (1 << (4 * pdl_number + 2))) == 0 )
	{
		return false;
	}
	else
	{
		return true;
	}
}


#endif // PDL_H_INCLUDED
