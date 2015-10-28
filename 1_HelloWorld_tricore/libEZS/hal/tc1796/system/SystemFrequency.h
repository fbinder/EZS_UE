/*====================================================================
* Function: Basic abstraction for TriCore TC1796 System Frequency
* Description:
*
* Copyright I4Copter Project 2007-2010
* http://www4.informatik.uni-erlangen.de/Research/I4Copter
*=====================================================================
* Author: Peter Ulbrich, Florian Franzmann, Christian Franzen
*=====================================================================
* $Id: SystemFrequency.h 2966 2010-07-12 14:27:05Z ulbrich $
*====================================================================*/
#ifndef SYSTEMFREQUENCY_H
#define SYSTEMFREQUENCY_H

#include <stdint.h>


class SystemFrequency {
    private:
        static SystemFrequency sm_instance;
        uint32_t m_cpuFrequency;
        uint32_t m_sysFrequency;
        uint32_t m_stmFrequency;

        /*!
        * \brief ctor
        */
        SystemFrequency() {}
        SystemFrequency(const SystemFrequency &other) {}

        /*!
        * \brief This method initializes the value
        * cpuFrequency() returns.
        *
        * \see uint32_t cpuFrequency() const
        */
        void initCpuFrequency();

        /*!
        * \brief This method initializes the value
        * sysFrequency() returns. Prerequisite: initCpuFrequency()
        * must have been called.
        *
        * \see uint32_t sysFrequency() const
        */
        void initSysFrequency();

        /*!
        * \brief This method initializes the value
        * stmFrequency() returns. Prerequisite: initSysFrequency
        * must have been called.
        *
        * \see uint32_t stmFrequency() const
        */
        void initStmFrequency();
    public:
        static SystemFrequency &Instance();

        /*!
        * \brief This method must be called exactly once before
        * any of the other public methods can be used.
        */
        void init();

        /*!
        * \brief This method returns the CPU's frequency.
        */
        uint32_t cpuFrequency() const;

        /*!
        * \brief This method returns the system's frequency.
        */
        uint32_t sysFrequency() const;

        /*!
        * \brief This method returns the system timer's frequency.
        */
        uint32_t stmFrequency() const;

};


#endif // SYSTEMFREQUENCY_H
