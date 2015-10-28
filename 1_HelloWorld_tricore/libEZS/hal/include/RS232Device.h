/**
 * Copyright I4Copter Project 2007-2011
 * http://www4.informatik.uni-erlangen.de/Research/I4Copter
 *
 * \file RS232Device.h
 * \ingroup hal
 * \brief RS232 Abstraction Layer
 * \author Martin Hoffmann <hoffmann@cs.fau.de>
 */


#ifndef RS232DEVICE_H
#define RS232DEVICE_H

#include <stdint.h>

//! \note The actual implementation is determined by the header search path, which depends on the CMake build type (microcontroller platform)
#include "RS232DeviceImpl.h"

namespace hal {

/**
 * \class RS232Device
 * \brief RS232 Interface Abstractions
 */
class RS232Device {
	RS232DeviceImpl m_impl;
public:

	//! A baudrate type..
	enum Baudrate {
		BR9600 = 9600,
		BR38400 = 38400,
		BR115200 = 115200,
	} ;

	/**
	 * Constructor defines the RS232 Device number
	 * @param device number
	 */
	RS232Device(uint8_t const device) : m_impl(device){};

	/**
	 * Initalizer mainly sets the baudrate
	 * @param baudrate
	 * @return true if initalization was successful
	 */
	bool init(RS232Device::Baudrate baudrate){ return m_impl.init(baudrate); };

	/**
	 * \brief Send a byte blocking
	 * @param b Byte to send
	 */
	void send(uint8_t b) { m_impl.send(b); };

	/**
	 * \brief Send an array of bytes (blocking)
	 *
	 * @param buf Array to be sent
	 * @param length Length of the array
	 * @return Number of bytes sent
	 */
	void send(uint8_t * buf, uint32_t length){ m_impl.send(buf, length); };


	/**
	 * \brief Send byte non-blocking
	 * \param b Byte to send
	 * \return \c true if byte is scheduled for sending, else \c false
	 */
	bool send_nb(uint8_t b){ return m_impl.send_nb(b); };

	/**
	 * \brief Send an array of bytes non-blocking
	 * \param buf Pointer to the array to send
	 * \param length Length of the array
	 * \return Number of bytes sent, -1 on error TODO
	 */
	bool send_nb(uint8_t const * buf, uint32_t length){ return  m_impl.send_nb(buf, length); };

	/**
	 * \brief Receive a single byte blocking
	 * @return The received byte
	 */
	uint8_t recv() { return m_impl.recv(); };
	/**
	 * \brief Enable RS232 RX Interrupt
	 */

	void enable_rx_int(){ m_impl.enable_rx_irq(); };

	/**
	 * \brief Disable RS232 RX Interrupt
	 */
	void disable_rx_int(){ m_impl.disable_rx_irq(); };

	/**
	 * \brief Received byte from within RX IRQ Handler
	 * @param dest Reference where to store the received byte
	 * @return true if byte was received, false on error
	 * \note The hw dependent implementation can reset status bits here.
	 */
	bool getc_irq(uint8_t& dest){ return  m_impl.getc_irq(dest); };

	/**
	 * \brief Setup DMA based reception of a string
	 *
	 * @param buffer The destination where to store the bytes
	 * @param length The number of bytes to receive
	 * \note The hw dependent implementation can reset status bits here.
	 * \note This method enables DMA Interrupt after complete reception! (\see dma_rx_completed_irq)
	 */
	void config_dma_rx_irq(uint8_t* buffer, uint32_t length){ m_impl.config_dma_rx_irq(buffer, length); };

	/**
	 * \brief Test and acknowledge dma rx interrupt
	 * @return \c true if IRQ was an DMA RX complete, \c false on error
	 * \note The hw dependent implementation can reset status bits here.
	 * \note This must be called from within the according IRQ Handler
	 */
	bool dma_rx_completed_irq()  { return m_impl.rx_dma_handler_completed(); };


	/**
	 * \brief Test and acknowledge dma tx interrupt
	 * @return \c true if IRQ was an DMA TX complete, \c false on error
	 * \note The hw dependent implementation can reset status bits here.
	 * \note This must be called from within the according IRQ Handler
	 */
	bool dma_tx_completed_irq()  { return m_impl.tx_dma_handler(); };


};


}


#endif
