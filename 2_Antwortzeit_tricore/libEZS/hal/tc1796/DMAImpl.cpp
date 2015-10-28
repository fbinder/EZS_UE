/**
 * Copyright I4Copter Project 2007-2013
 * http://www4.informatik.uni-erlangen.de/Research/I4Copter
 *
 * \file DMAImpl.h
 * \ingroup tc1796
 * \brief TC1796 specific interface for a DMA channel
 * \author Florian Lukas <florian.lukas@gmail.com>
 */

#include "DMAImpl.h"
#include <machine/wdtcon.h>
#include "OSAbstraction.h"
#include "hw/triboard1796.h" // WDT lock/unlock implemented in ciao

/// Static member initialization
DMA_t* const DMAChannelImpl::dmaBase = (DMA_t*) DMA_BASE;
hal::digitaldevice::callback DMAChannelImpl::callbacks[DMAChannelImpl::unit_count*DMAChannelImpl::channels_per_unit];
void* DMAChannelImpl::cb_arguments[DMAChannelImpl::unit_count*DMAChannelImpl::channels_per_unit];

void DMAChannelImpl::init() {
	// allow RAM and FLASH access
	allow_range(range_DPRAM | range_EXTRAM | range_PFLASH | range_DFLASH);
}

void DMAChannelImpl::allow_range(uint32_t range) {
	hw::_tc::unlock_wdtcon();
	((m_unit == 0) ? dmaBase->ME0AENR : dmaBase->ME1AENR).reg |= range;
	hw::_tc::lock_wdtcon();
}

void DMAChannelImpl::set_source(uint32_t* src_addr, bool increment) {
	dmaBase->CH[m_channel].SADR.reg = reinterpret_cast<uint32_t>(src_addr); // source address
	dmaBase->CH[m_channel].ADRCR.bits.INCS = 1; // increment mode
	dmaBase->CH[m_channel].ADRCR.bits.CBLS = increment ? 15 : 0; // increment last 15 bits if specified
}

void DMAChannelImpl::set_destination(uint32_t* dest_addr, bool increment) {
	dmaBase->CH[m_channel].DADR.reg = reinterpret_cast<uint32_t>(dest_addr); // destination address
	dmaBase->CH[m_channel].ADRCR.bits.INCD = 1; // increment mode
	dmaBase->CH[m_channel].ADRCR.bits.CBLD = increment ? 15 : 0; // increment last 15 bits if specified
}

void DMAChannelImpl::set_length(uint32_t length, move_size movesize) {
	dmaBase->CH[m_channel].CHCR.bits.TREL = length; // move count/transaction length
	dmaBase->CH[m_channel].CHCR.bits.CHDW = movesize; // size of move in bits
}

void DMAChannelImpl::start() {
	dmaBase->CH[m_channel].CHCR.bits.RROAT = 1; // reset after complete transaction
	dmaBase->STREQ.reg |= 1 << m_channel; // set request flag
}

bool DMAChannelImpl::done() {
	return (dmaBase->TRSR.reg & (1 << m_channel)) == 0; // check status (running) flag
}

void DMAChannelImpl::set_irq_handler(hal::dma::callback cb, void* arg) {
	uint8_t idx = m_unit*channels_per_unit + m_channel;
	callbacks[idx] = cb;
	cb_arguments[idx] = arg;
}

void DMAChannelImpl::set_irq(bool enabled) {
	dmaBase->CH[m_channel].CHICR.bits.INTCT |= 2; // interrupt when TCOUNT==IRDV
	dmaBase->CH[m_channel].CHICR.bits.INTP = 0; // use DMA_SRC0, TODO: make configurable?

	DMA_SRCm_t* src = &dmaBase->SRC0 - dmaBase->CH[m_channel].CHICR.bits.INTP;
	src->bits.SRE = 1; // service request enable
}

void DMAChannelImpl::irq_handler(void) {
	uint16_t cmask = dmaBase->INTSR.reg & 0xFFFF; // finished channel number
	while(cmask != 0) {
		uint8_t chan = 31 - _clz(cmask); // get first set bit from cmask
		dmaBase->INTCR.reg |= (1 << chan); // clear interrupt flag
		cmask &= ~(1 << chan);
		callbacks[chan](cb_arguments[chan]);
	}
}

