/**
 * Copyright I4Copter Project 2007-2011
 * http://www4.informatik.uni-erlangen.de/Research/I4Copter
 *
 * \file SPI.h
 * \ingroup hal
 * \brief SPI Abstraction Layer
 * \author Martin Hoffmann <hoffmann@cs.fau.de>
 */

#ifndef SPIBUS_H_
#define SPIBUS_H_

#include <stdint.h>
#include "DigitalDevice.h"
//! \note The actual implementation is determined by the header search path, which depends on the CMake build type (microcontroller platform)
#include "Config.h"
#include "SPIBusImpl.h"

namespace hal {

/**
 * \class SPIBus
 * \brief SPI Interface Abstractions
 *
 * This is the basic "abstract" class of the
 * SPI interface (some sort of Pimpl pattern).
 * A SPI::Impl class
 * holds the hardware dependent implementation.
 */
class SPIBus {
	SPIBusImpl* m_impl;
public:

	SPIBus(uint8_t const device)  {
		if(device == 1) m_impl = &spibus1;
		if(device == 2) m_impl = &spibus2;
	};
	/**
	 * \brief Interface initialization
	 *
	 * This method just calls the init method
	 * of a global SPI::Impl "Singleton"
	 */
	void init( hal::spi::Mode_t mode) const { m_impl->init(mode); }


	/**
	 * \brief SPI send routine
	 * @param address SPI Slave address
	 * @param data 	  Buffer to read data to send
	 * @param length  Buffer/Message length
	 * @return
	 */
	uint32_t master_send(DigitalDevice& slave, uint8_t* txbuf, uint8_t* rxbuf, uint32_t length, hal::spi::tx_callback cb) const {
		return m_impl->send_nonblocking(slave, txbuf, rxbuf, length, cb);
	}


	/**
	 * \brief SPI slave receive routine
	 * @param address Slave Select line to wait for data
	 * @param data 	  Buffer to input/output data
	 * @param length  Buffer/Message length
	 * @return
	 */
	uint32_t slave_receive(DigitalDevice& slave, uint8_t* rxbuf, uint8_t* txbuf, uint32_t length, hal::spi::rx_callback cb) const {
		return m_impl->receive_nonblocking(slave, rxbuf, txbuf, length, cb);
	}
};

} // end of namespace

#endif /* SPIBUS_H_ */
