/**
 * \defgroup hal General hardware abstraction layer
 */

/**
 * \defgroup tc1796 TC1796 hardware abstraction layer
 * \ingroup hal
 */

/**
 * \defgroup stm32f1 STM32F1xx hardware abstraction layer
 * \ingroup hal
 */

/**
 * \namespace hal
 * \brief Namespace for the hardware abstraction layer
 */

/**
 * \namespace tc1796
 * \brief Namespace for the TriCore TC1796 specific part of the hardware abstraction layer
 */

/**
 * \namespace stm32f1
 * \brief Namespace for the STM32F1 specific part of the hardware abstraction layer
 */

/**
 * Copyright I4Copter Project 2007-2011
 * http://www4.informatik.uni-erlangen.de/Research/I4Copter
 *
 * \file HalStartup.h
 * \ingroup hal
 * \brief All HAL Startup routines are called here.
 * \author Peter Ulbrich <ulbrich@cs.fau.de>
 * \author Martin Hoffmann <hoffmann@cs.fau.de>
 */

#ifndef HALSTARTUP_H
#define HALSTARTUP_H

namespace hal {

/**
 * \class HalStartup
 *
 * \brief The abstract interface for the HAL startup
 *
 * This class implements a run method.
 */
class HalStartup {
public:
	/**
	 * This is the basic run method for proper
	 * hal initialization
	 */
	static void run(void);

protected:
private:
};

}
#endif // HALSTARTUP_H
