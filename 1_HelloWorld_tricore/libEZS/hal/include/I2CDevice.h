/**
 * Copyright I4Copter Project 2007-2011
 * http://www4.informatik.uni-erlangen.de/Research/I4Copter
 *
 * \file I2C.h
 * \ingroup hal
 * \brief I2C Abstraction Layer
 * \author Martin Hoffmann <hoffmann@cs.fau.de>
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

//! \note The actual implementation is determined by the header search path, which depends on the CMake build type (microcontroller platform)
#include "I2CDeviceImpl.h"

namespace hal {

/**
 * \class I2C
 * \brief I2C Interface Abstractions
 *
 * This is the basic "abstract" class of the
 * I2C interface (some sort of Pimpl pattern).
 * A I2C::Impl class
 * holds the hardware dependent implementation.
 */
class I2C {

public:
	/**
	 * \brief Interface initialization
	 *
	 * This method just calls the init method
	 * of a global I2C::Impl "Singleton"
	 */
	static void init() { i2cDeviceImpl.init(); }


	/**
	 * \brief I2C send routine
	 * @param address I2C Slave address
	 * @param data 	  Buffer to read data to send
	 * @param length  Buffer/Message length
	 * @return
	 */
	static uint32_t send(uint8_t address, uint8_t* data, uint32_t length) {
		return i2cDeviceImpl.send(address, data, length);
	}

	/**
	 * \brief I2C receive routine
	 * @param address I2C Slave address
	 * @param data 	  Buffer to write received data to
	 * @param length  Buffer/Message length
	 * @return
	 */
	static uint32_t receive(uint8_t address, uint8_t* data, uint32_t length) {
		return i2cDeviceImpl.receive(address, data, length);
	}
};

} // end of namespace

#endif /* I2C_H_ */
