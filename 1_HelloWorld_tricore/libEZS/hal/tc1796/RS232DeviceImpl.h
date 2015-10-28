/**
 * Copyright I4Copter Project 2007-2011
 * http://www4.informatik.uni-erlangen.de/Research/I4Copter
 *
 * \file RS232DeviceImpl.h
 * \ingroup tc1796
 * \brief TC1796 specific interface for an analog device
 * \author Matthias Brischwein <matthias.brischwein@studium.uni-erlangen.de>
 * \addtogroup tc1796
 * @{
 */

#ifndef RS232DEVICEIMPL_H_
#define RS232DEVICEIMPL_H_

#include <stdint.h>
//#include "GPIO.h"

#include <machine/wdtcon.h>

#include <tc1796b/p5.h>
#include <tc1796b/p6.h>
#include <tc1796b/asc0.h>
#include <tc1796b/asc1.h>
#include <tc1796b/asc0-struct.h>
#include <tc1796b/asc1-struct.h>

namespace hal { namespace tc1796 {

class RS232DeviceImpl {

public:

	RS232DeviceImpl(uint8_t const number) : m_unitId(number) {

	};

	bool init(uint32_t baudrate);


	/**
	 * \brief Send a byte blocking
	 * @param b Byte to send
	 */
	void send(uint8_t b);

	/**
	 * \brief Send an array of bytes (blocking)
	 *
	 * @param buf Array to be sent
	 * @param length Length of the array
	 * @return Number of bytes sent
	 */
	void send(uint8_t * buf, uint32_t length);

	/**
	 * \brief Send byte non-blocking
	 * \param b Byte to send
	 * \return \c true if byte is scheduled for sending, else \c false
	 */
	bool send_nb(uint8_t b);

	/**
	 * \brief Send an array of bytes non-blocking
	 * \param buf Pointer to the array to send
	 * \param length Length of the array
	 * \return Number of bytes sent, -1 on error TODO
	 */
	bool send_nb(uint8_t const * buf, uint32_t length);

	/**
	 * \brief Receive a single byte blocking
	 * @return The received byte
	 */
	uint8_t recv() const;

	/**
	 * \brief Enable RS232 RX Interrupt
	 */
	void enable_rx_irq();

	/**
	 * \brief Disable RS232 RX Interrupt
	 */
	void disable_rx_irq();

	/**
	 * \brief Interrupt handler for RS232 RX
	 */
	void rx_irq_handler();

	/**
	 * \brief Interrupt handler for RS232 (DMA ready)
	 */
	bool rx_dma_handler_completed();

	/**
	 * \brief Interrupt handler for RS232 TX (DMA done)
	 */
	bool tx_dma_handler();

	/**
	 * \brief Setup DMA based reception of a string
	 *
	 * @param buffer The destination where to store the bytes
	 * @param length The number of bytes to receive
	 * \note This method enables DMA Interrupt after complete reception! (\see rx_dma_handler)
	 */
	void config_dma_rx_irq(uint8_t* buffer, uint32_t length);

	/**
	 * \brief Reset status bit and return received byte
	 * @param dest Reference where to store the received byte
	 * @return true if byte was received, false on error
	 * \note this should be called within an interrupt handler
	 */
	bool getc_irq(uint8_t& dest);

protected:

private:
	const uint8_t m_unitId;

	enum {
		ASC0_ID = 0,
		ASC1_ID = 1,
	};

	volatile ASC0_t *m_pASC0;
	volatile ASC1_t *m_pASC1;
};


} // ::tc1796
} // ::hal

/**
 * @note Workaround for the different abstraction layers to match the abstract hal namespace. Example: AnalogDevice.h uses hal:AnalogDeviceImpl
 */
using namespace hal::tc1796;

#endif /* RS232DEVICEIMPL_H_ */
