/**
 * Copyright I4Copter Project 2007-2012
 * http://www4.informatik.uni-erlangen.de/Research/I4Copter
 *
 * \file AnalogDevice.h
 * \ingroup hal
 * \brief Abstract interface for an analog device
 * \author Peter Ulbrich <ulbrich@cs.fau.de>
 *
 */

#ifndef ANALOGDEVICE_H_
#define ANALOGDEVICE_H_

#include <stdint.h>
//! \note The actual implementation is determined by the header search path, which depends on the CMake build type (microcontroller platform)
#include "AnalogDeviceImpl.h"

namespace hal {

/**
 * \class AnalogDevice
 * \brief Abstract interface for an analog device (pin)
 * \note This class is just a wrapper for the platform specific implementation (AnalogDeviceImpl)
 */
class AnalogDevice {
public:
	/**
	 * constructor...
	 * @param pin
	 */
	AnalogDevice(AnalogDeviceImpl::channelnum const pin) :
			m_impl(pin) {
	}

	/**
	 * Initialize implementation object
	 * \todo Do this explicitly or implicitly in the constructor?
	 * @return
	 */
	bool init() {
		return m_impl.init();
	}

	/**
	 * Trigger the sampling of the analog pin
	 * \note Even in case of autoscan/DMA operation, this is a necessary step. Otherwise the raw and the millivolt value would differ!
	 */
	void sample() {
		m_value = m_impl.sample();
		m_millivolt = toMillivolt(m_value);
	}

	/**
	 * Convert a analog raw value to millivolt
	 * \note It is assumed that the raw value has a 12bit resolution (3300mV = 4095raw). The conversion factor is thereof 0.80581!
	 * @param value A raw value from a 3.3V ADC
	 * @return Millivolt
	 */
	float toMillivolt(const int16_t value) const {
		return static_cast<float>(value) * 0.80581f;
	}

	/**
	 * Get the sampled raw value of the analog pin
	 * \waring sample() must be called before reading the result!
	 * @return A raw sample value
	 */
	uint16_t getValue() const {
		return m_value;
	}

	/**
	 * Get the sampled value of the analog pin
	 * \waring sample() must be called before reading the result!
	 * @return A sample value in millivolt
	 */
	float getMillivolt() const {
		return m_millivolt;
	}

protected:
	// Raw value
		uint16_t m_value;

	// millivolt
		float m_millivolt;

private:
//! HW specific implementation object
	AnalogDeviceImpl m_impl;

};
// End of class AnalogDevice

/**
 * \class AnalogDeviceBiased
 * \brief Abstract interface for an biased analog device (pin)
 * \note This class is just a wrapper for the platform specific implementation (AnalogDeviceImpl)
 */
class AnalogDeviceBiased: protected AnalogDevice {
public:

	/**
	 * constructor...
	 * @param pin
	 * @param calCycles Number of cycles to be used for calibaration
	 */
	AnalogDeviceBiased(AnalogDeviceImpl::channelnum const pin,
			uint16_t calCycles) :
			AnalogDevice(pin), m_calCycles(calCycles) {
		reset();
	}

	/**
	 * Initialize implementation object
	 * \todo Do this explicitly or implicitly in the constructor?
	 * @return
	 */
	bool init() {
		reset();
		return AnalogDevice::init();
	}

	void reset() {
		m_isBiased = false;
		m_iterator = 0;
		m_biasSum = 0;
		m_bias = 0;
	}

	bool sample() {
		AnalogDevice::sample();

		if (!m_isBiased) {
			calibrateBias();
		} else {
			m_valueBiased = AnalogDevice::getValue() - m_bias;
			m_millivoltBiased = AnalogDevice::toMillivolt(m_valueBiased);
		}

		return m_isBiased;
	}

	int16_t getValue() const {
		return m_valueBiased;
	}

	float getMillivolt() const {
		return m_millivoltBiased;
	}

	uint16_t getValue_ub() const {
		return AnalogDevice::getValue();
	}

	float getMillivolt_ub() const {
		return AnalogDevice::getMillivolt();
	}

	uint16_t getBias() const {
		return m_bias;
	}

protected:

private:
	// Bias calibration data
	const uint16_t m_calCycles;
	uint16_t m_iterator;
	uint32_t m_biasSum;
	bool m_isBiased;

	// Bias
	uint16_t m_bias;

	// Biased values
	int16_t m_valueBiased;
	float m_millivoltBiased;

	void calibrateBias() {
		m_iterator++;
		m_biasSum += AnalogDevice::getValue();

		if (m_iterator == m_calCycles) {
			m_bias = m_biasSum / m_calCycles;
			m_isBiased = true;
		}
	}

};
// End of class AnalogDeviceBiased


} // End of namespace hal

#endif /* ANALOGDEVICE_H_ */
