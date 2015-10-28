/**
 * Copyright I4Copter Project 2007-2013
 * http://www4.informatik.uni-erlangen.de/Research/I4Copter
 *
 * \file DMAImpl.h
 * \ingroup tc1796
 * \brief TC1796 specific interface for a DMA channel
 * \author Florian Lukas <florian.lukas@gmail.com>
 * \addtogroup tc1796
 * @{
 */

#ifndef DMAIMPL_H_
#define DMAIMPL_H_

#include <stdint.h>
#include <tc1796b/dma-struct.h>
#include <tc1796b/dma.h>
#include <tc1796b/dma/sharemasks.h>
#include "Config.h"

namespace hal { namespace tc1796 {

/**
 * \class DMAChannelImpl
 * \ingroup tc1796
 * \brief TC1796 specific interface for a DMA channel
 */
class DMAChannelImpl {
public:
	static const uint8_t unit_count = 2;
	static const uint8_t channels_per_unit = 8;

	/**
	 * DMA unit number
	 */
	enum unitnum {
		dma0 = 0,//!< DMA 0
		dma1 = 1,//!< DMA 1
	};

	/**
	 * DMA channel number
	 */
	enum channelnum {
		chan0 = 0,//!< Channel 0
		chan1 = 1,//!< Channel 1
		chan2 = 2,//!< Channel 2
		chan3 = 3,//!< Channel 3
		chan4 = 4,//!< Channel 4
		chan5 = 5,//!< Channel 5
		chan6 = 6,//!< Channel 6
		chan7 = 7,//!< Channel 7
	};

	/**
	 * DMA move size
	 */
	enum move_size {
		move_8bit  = 0,//!< 8-Bit moves
		move_16bit = 1,//!< 16-Bit moves
		move_32bit = 2,//!< 32-Bit moves
	};

	/**
	 * DMA access protection ranges (TC1796)
	 */
	enum address_range {
		range_ASC0 = DMA_MEmAENR_AEN5_MASK,   //!< ASC0
		range_ASC1 = DMA_MEmAENR_AEN6_MASK,   //!< ASC1
		range_SSC0 = DMA_MEmAENR_AEN14_MASK,  //!< SSC0
		range_SSC1 = DMA_MEmAENR_AEN15_MASK,  //!< SSC1
		range_DPRAM = DMA_MEmAENR_AEN18_MASK, //!< DPRAM
		range_PFLASH = DMA_MEmAENR_AEN23_MASK,//!< Program FLASH
		range_EXTRAM = DMA_MEmAENR_AEN24_MASK,//!< External RAM
		range_DFLASH = DMA_MEmAENR_AEN25_MASK,//!< Data FLASH
		range_EXTPER = DMA_MEmAENR_AEN28_MASK,//!< External Periperals
	};

	/**
	 * DMA channel constructor.
	 * @param unit DMA unit this channel belongs to
	 * @param channel DMA channel number (in DMA unit)
	 */
	DMAChannelImpl(unitnum const unit, channelnum const channel) : m_unit(unit), m_channel(channel) {};

	/**
	 * Initialize DMA.
	 */
	void init();

	/**
	 * Allow DMA access to memory range.
	 */
	void allow_range(uint32_t const range);

	/**
	 * Set DMA transfer source address and increment mode.
	 * @param src_addr source address
	 * @param increment increment source address after each move (0: always read same address)
	 */
	void set_source(uint32_t* src_addr, bool increment);

	/**
	 * Set DMA transfer destination address and increment mode.
	 * @param dest_addr destination address
	 * @param increment increment destination address after each move (0: always write same address)
	 */
	void set_destination(uint32_t* dest_addr, bool increment);

	/**
	 * Set DMA transfer length and move unit.
	 * @param length transfer length in number of moves
	 * @param size of one DMA move
	 */
	void set_length(uint32_t length, move_size movesize);

	/**
	 * Request start of DMA transaction.
	 */
	void start();

	/**
	 * Check if DMA transfer is done.
	 */
	bool done();

	/**
	 * Set IRQ handler to be called (with argument) when DMA transaction is finished
	 * @param cb call
	 * @param arg
	 */
	void set_irq_handler(hal::dma::callback cb, void* arg);

	/**
	 * Enable/disable DMA finish interrupt.
	 * @param enabled enable (true) or disable (false) IRQ
	 */
	void set_irq(bool enabled);

	/**
	 * IRQ handler to be called by ISR on transaction finish
	 */
	static void irq_handler(void);

private:
	/**
	 * DMA unit number
	 */
	const unitnum m_unit;

	/**
	 * DMA channel number
	 */
	const channelnum m_channel;

	/**
	 * Callbacks (for all channels)
	 */
	static hal::digitaldevice::callback callbacks[unit_count*channels_per_unit];

	/**
	 * Callback arguments (for all channels)
	 */
	static void* cb_arguments[unit_count*channels_per_unit];

	/**
	 * Base address of the DMA unit
	 */
	static DMA_t* const dmaBase;
}; // End DMAChannelImpl

} // End namespace tc1796
} // End namespace hal

/**
 * @note Workaround for the different abstraction layers to match the abstract hal namespace. Example: AnalogDevice.h uses hal:AnalogDeviceImpl
 */
using namespace hal::tc1796;

#endif /* DMAIMPL_H_ */

/**
 * @} End of doxygen group
 */
