/**
 * Copyright I4Copter Project 2007-2011
 * http://www4.informatik.uni-erlangen.de/Research/I4Copter
 *
 * \file
 * \ingroup tc1796
 * \brief TC1796 specific interface for an analog device
 * \author Peter Ulbrich <ulbrich@cs.fau.de>
 * \author Martin Hoffmann <hoffmann@cs.fau.de>
 * \addtogroup tc1796
 * @{
 */

#ifndef ANALOGDEVICEIMPL_H_
#define ANALOGDEVICEIMPL_H_

#include "adc/ADC.h"

namespace hal { namespace tc1796 {

/**
 * \class AnalogDeviceImpl
 * \ingroup tc1796
 * \brief TC1796 specific interface for an analog device (pin)
 */
class AnalogDeviceImpl{
public:

	enum channelnum {
		adc_chan0  = 0,
		adc_chan1  = 1,
	};

	AnalogDeviceImpl(channelnum const channel) : m_channelnum(channel) {};

	bool init();
	uint16_t sample() const;

private:

	AnalogDeviceImpl() {};

	channelnum m_channelnum;
	ADC<0> m_adc_channel_0;
	ADC<1> m_adc_channel_1;

}; // End AnalogDeviceImpl

} // End namespace tc1796
} // End namespace hal


/**
 * @note Workaround for the different abstraction layers to match the abstract hal namespace. Example: AnalogDevice.h uses hal:AnalogDeviceImpl
 */
using namespace hal::tc1796;

#endif /* ANALOGDEVICEIMPL_H_ */

/**
 * @} End of doxygen group
 */
