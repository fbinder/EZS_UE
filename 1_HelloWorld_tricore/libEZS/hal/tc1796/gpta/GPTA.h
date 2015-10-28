/*====================================================================
* Function: Basic abstraction for TriCore TC1796 GPTA
* Description:
*
* Copyright I4Copter Project 2007-2010
* http://www4.informatik.uni-erlangen.de/Research/I4Copter
*=====================================================================
* Author: Peter Ulbrich, Adriaan Schmid, Christian Franzen
*=====================================================================
* $Id: GPTA.h 2966 2010-07-12 14:27:05Z ulbrich $
*====================================================================*/
#ifndef GPTA_H_INCLUDED
#define GPTA_H_INCLUDED

#include <stdint.h>

class GPTA
{
    public:
        /**
         * \brief Initialise the GPTA module
         * \param frequency The base frequency to be set (in Hz)
         * \warning Note that you must enable the GPTAs with a call to
         *          initModule()
         */
        static void init(uint32_t frequency);

        /**
         * \brief Enable a GPTA
         * \param gpta_number The number of the GPTA to be initialised (0 or 1)
         * \return true on success, false on error
         */
        static bool initModule(uint8_t gpta_number);

        /**
         * \brief Get the base frequency of the GPTA's
         * \return The base frequency of the GPTA's (in Hz)
         */
        static uint32_t getFrequency();

        /**
         * \brief Get the time needed by the GPTA's for one tick
         * \return The time for a tick (in s)
         */
        static float getTimePerTick();

    private:
        static uint32_t m_iFreq;
};

#endif // GPTA_H_INCLUDED
