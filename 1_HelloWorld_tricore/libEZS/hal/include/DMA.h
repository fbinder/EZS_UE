/**
 * Copyright I4Copter Project 2007-2013
 * http://www4.informatik.uni-erlangen.de/Research/I4Copter
 *
 * \file DMA.h
 * \ingroup hal
 * \brief Abstract interface for a DMA channel
 * \author Florian Lukas <florian.lukas@gmail.com>
 *
 */

#ifndef DMA_H_
#define DMA_H_

#include <stdint.h>
#include "Config.h"
//! \note The actual implementation is determined by the header search path, which depends on the CMake build type (microcontroller platform)
#include "DMAImpl.h"

namespace hal {

/**
 * \class DMAChannel
 * \brief Abstract interface for a DMA channel
 * \note This class is just a wrapper for the platform specific implementation (DMAChannelImpl)
 */
class DMAChannel {
public:
	typedef DMAChannelImpl::unitnum unitnum;
	typedef DMAChannelImpl::channelnum channelnum;
	typedef DMAChannelImpl::move_size move_size;

	/**
	 * Constructor
	 * @param channel DMA channel number
	 */
	DMAChannel(DMAChannelImpl::unitnum const unit, DMAChannelImpl::channelnum const channel) : m_impl(unit, channel) {}

	/**
	 * Initialize DMA.
	 */
	void init() {
		m_impl.init();
	}

	/**
	 * Set DMA transfer source address and increment mode.
	 * @param src_addr source address
	 * @param increment increment source address after each move (0: always read same address)
	 */
	void set_source(uint32_t* src_addr, bool increment) {
		m_impl.set_source(src_addr, increment);
	}

	/**
	 * Set DMA transfer destination address and increment mode.
	 * @param dest_addr destination address
	 * @param increment increment destination address after each move (0: always write same address)
	 */
	void set_destination(uint32_t* dest_addr, bool increment) {
		m_impl.set_destination(dest_addr, increment);
	}

	/**
	 * Set DMA transfer length and move unit.
	 * @param length transfer length in number of moves
	 * @param size of one DMA move
	 */
	void set_length(uint32_t length, move_size movesize) {
		m_impl.set_length(length, movesize);
	}

	/**
	 * Request start of DMA transaction.
	 */
	void start() {
		m_impl.start();
	}

	/**
	 * Check if DMA transfer is done.
	 */
	bool done() {
		return m_impl.done();
	}

	/**
	 * Start DMA transaction and BLOCK until finish.
	 */
	void start_block() {
		start();
		while(!done()) {};
	}

	/**
	 * Set IRQ handler to be called (with argument) when DMA transaction is finished
	 * @param cb call
	 * @param arg
	 */
	void set_irq_handler(hal::dma::callback cb, void* arg) {
		m_impl.set_irq_handler(cb, arg);
	}

	/**
	 * Enable DMA finish interrupt.
	 */
	void enable_irq() {
		m_impl.set_irq(true);
	}

	/**
	 * Disable DMA finish interrupt.
	 */
	void disable_irq(){
		m_impl.set_irq(false);
	}

private:
	//! HW specific implementation object
	DMAChannelImpl m_impl;
}; // End of class DMAChannel

} // End of namespace hal

#endif /* DMA_H_ */
