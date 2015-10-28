/**
 * Copyright I4Copter Project 2007-2011
 * http://www4.informatik.uni-erlangen.de/Research/I4Copter
 */

#include "AnalogDeviceImpl.h"


bool AnalogDeviceImpl::init()
{
	switch(m_channelnum) {
	case adc_chan0:
		m_adc_channel_0.init();
		m_adc_channel_0.setupScanmode(ADC_SCANMODE_CONTINUOUS);
		break;
	case adc_chan1:
		m_adc_channel_1.init();
		m_adc_channel_1.setupScanmode(ADC_SCANMODE_CONTINUOUS);
		break;
	}
	return true;
}


uint16_t AnalogDeviceImpl::sample() const
{
}

