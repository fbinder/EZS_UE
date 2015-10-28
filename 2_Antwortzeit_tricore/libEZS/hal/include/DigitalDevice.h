/**
 * Copyright I4Copter Project 2007-2012
 * http://www4.informatik.uni-erlangen.de/Research/I4Copter
 *
 * \file DigitalDevice.h
 * \ingroup hal
 * \brief Abstract interface for an digital device (I/O Pin)
 * \author Martin Hoffmann <hoffmanh@cs.fau.de>
 *
 */

#ifndef DIGITALDEVICE_H_
#define DIGITALDEVICE_H_

#include "Config.h"
//! \note The actual implementation is determined by the header search path, which depends on the CMake build type (microcontroller platform)
#include "DigitalDeviceImpl.h"

namespace hal {

/**
 * \class DigitalDevice
 * \brief Abstract interface for an digital device (pin)
 * \note This class is just a wrapper for the platform specific implementation (DigitalDeviceImpl)
 */
class DigitalDevice {
public:
	/**
	 * constructor...
	 * @param pin
	 */
	DigitalDevice(DigitalDeviceImpl::pin_t const pin, hal::digitaldevice::PINDIR_t dir) :
			m_impl(pin, dir) {
	}
	/**
	 * constructor...
	 * @param pin
	 */
	DigitalDevice(DigitalDeviceImpl::pin_t const pin) :
			m_impl(pin, hal::digitaldevice::Output) {
	}


	/**
	 * Initialize implementation object
	 */
	bool init()  {
		return m_impl.init();
	}

	/**
	 * Set the pin to hight
	 */
	void set() const {
		m_impl.set();
	}


	/**
	 * Set the pin to hight
	 */
	void clear() const {
		m_impl.clear();
	}

	/**
	 * Toggle pin
	 * @return The current pin level
	 */
	bool toggle() const {
		return m_impl.toggle();
	}

	void set_irq_handler(hal::digitaldevice::callback cb, void* arg)  {
		m_impl.set_irq_handler(cb, arg);
	}

	void enable_irq(hal::digitaldevice::IRQDIR_t dir){
		m_impl.enable_irq(dir);
	}


	void disable_irq(){
		m_impl.disable_irq();
	}

	bool isSet(){
		return m_impl.isSet();
	}
private:
//! HW specific implementation object
	DigitalDeviceImpl m_impl;

};
// End of class AnalogDevice


} // End of namespace hal

#endif /* DIGITALDEVICE_H_ */
