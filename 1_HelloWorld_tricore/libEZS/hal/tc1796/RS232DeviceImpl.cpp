/**
 * Copyright I4Copter Project 2007-2011
 * http://www4.informatik.uni-erlangen.de/Research/I4Copter
 *
 * \file RS232DeviceImpl.cpp
 * \ingroup tc1796
 * \brief TC1796 specific interface for an analog device
 * \author uq43ywiw
 * \addtogroup tc1796
 * @{
 */

#include "RS232DeviceImpl.h"
#include "SystemFrequency.h"
#include <machine/wdtcon.h>

bool RS232DeviceImpl::init(uint32_t baudrate){
    unsigned long frequency, reload_value, fdv, dfreq;

    ASCn_CON_t *asc_con;
    ASCn_BG_t *asc_bg;
    ASCn_FDV_t *asc_fdv;
    ASCn_TSRC_t *asc_tsrc;
    ASCn_TBSRC_t *asc_tbsrc;

    // enable ASC
    unlock_wdtcon();
    ASC0_CLC.bits.RMC = 1;
    ASC0_CLC.bits.DISR = 0;
    lock_wdtcon();


    // calculate frequency settings
    frequency = SystemFrequency::Instance().sysFrequency();
    reload_value = (frequency / (baudrate * 16)) - 1;
    dfreq = frequency / (16*512);
    fdv = (reload_value + 1) * baudrate / dfreq;

    switch (m_unitId)
    {
    case ASC0_ID:
        // configure port pins
        ASC0_PISEL.reg = 0;
        // GPIO::setFunction(5, 0, GPIO_FUNCTION_INPUT_PULLUP);
        P5_IOCR0.bits.PC0 	= 0x2;  // 2 = GPIO_FUNCTION_INPUT_PULLUP
        // GPIO::setFunction(5, 1, GPIO_FUNCTION_ASC0);
        P5_IOCR0.bits.PC1 	= 0x9;	// 1001 = Port Output Alternate 1 Function Push/Pull
        // GPIO::setOutput(5, 1, 1);
        P5_OMR.bits.PS1 	= 1;

        m_pASC0 = (volatile ASC0_t*) ASC0_BASE;
        asc_con = &m_pASC0->CON;
        asc_bg = &m_pASC0->BG;
        asc_fdv = &m_pASC0->FDV;
        asc_tsrc = &m_pASC0->TSRC;
        asc_tbsrc = &m_pASC0->TBSRC;
        break;

    case ASC1_ID:
        // configure port pins
        ASC1_PISEL.reg = 1;
        // GPIO::setFunction(6, 10, GPIO_FUNCTION_INPUT_PULLUP);
        P6_IOCR8.bits.PC2 	= 0x2;  // 2 = GPIO_FUNCTION_INPUT_PULLUP
        // GPIO::setFunction(6, 11, GPIO_FUNCTION_ASC1); (CiAO ASC-HAL uses P5.3)
        P6_IOCR8.bits.PC3 	= 0x9;	// 1001 = Port Output Alternate 1 Function Push/Pull
        // GPIO::setOutput(6, 11, 1);
        P6_OMR.bits.PS11 	= 1;

        m_pASC1 = (volatile ASC1_t*) ASC1_BASE;
        asc_con = &m_pASC1->CON;
        asc_bg = &m_pASC1->BG;
        asc_fdv = &m_pASC1->FDV;
        asc_tsrc = &m_pASC1->TSRC;
        asc_tbsrc = &m_pASC1->TBSRC;
        break;

    default:
        return false;
    }

    // program ASC0
    asc_con->reg = 0;
    asc_bg->reg  = reload_value;
    asc_fdv->reg = fdv;

    asc_con->bits.M = 1;	// 8 Bit Data Async. mode
    asc_con->bits.R = 1; // enable baud rate generator
    asc_con->bits.REN = 1; // enable RX
    asc_con->bits.FDE = 1; // enable fractional divider for baudrate pre-scaling

    // start tx
    asc_tsrc->bits.SETR = 1;
    asc_tbsrc->bits.SETR = 1;

    return true;
}


/**
 * \brief Send a byte blocking
 * @param b Byte to send
 */
void RS232DeviceImpl::send(uint8_t b){
	switch (m_unitId) {
	case ASC0_ID:
		// Clear transmit buffer request flag
		ASC0_TBSRC.bits.CLRR = 1;
		// Move character to buffer
		ASC0_TBUF.bits.TD_VALUE = b;
		// Wait for transmition completed
		while (ASC0_TSRC.bits.SRR == 0)
			;
		// Clear transmit request flag
		ASC0_TSRC.bits.CLRR = 1;

		break;
	case ASC1_ID:
		ASC1_TBSRC.bits.CLRR = 1;
		ASC1_TBUF.bits.TD_VALUE = b;
		while (ASC1_TSRC.bits.SRR == 0)
			;
		ASC1_TSRC.bits.CLRR = 1;

		break;
	default:
		return;
	}
}

/**
 * \brief Send an array of bytes (blocking)
 *
 * @param buf Array to be sent
 * @param length Length of the array
 * @return Number of bytes sent TODO
 */
void RS232DeviceImpl::send(uint8_t * buf, uint32_t length){
	if(buf == 0) return;

	for(uint32_t i = 0; i < length; ++i) {
	  send(buf[i]);
	}
}

/**
 * \brief Send byte non-blocking
 * \param b Byte to send
 * \return \c true if byte is scheduled for sending, else \c false
 */
bool RS232DeviceImpl::send_nb(uint8_t b){
	// TODO
  return false;
}

/**
 * \brief Send an array of bytes non-blocking
 * \param buf Pointer to the array to send
 * \param length Length of the array
 * \return \c true on success \c false on failure
 */
bool RS232DeviceImpl::send_nb(uint8_t const * buf, uint32_t length){
  return false;
}

/**
 * \brief Receive a single byte blocking
 * @return The received byte
 */
uint8_t RS232DeviceImpl::recv() const{
  return false;
}

/**
 * \brief Enable RS232 RX Interrupt
 */
void RS232DeviceImpl::enable_rx_irq() {}

/**
 * \brief Disable RS232 RX Interrupt
 */
void RS232DeviceImpl::disable_rx_irq() {	}

/**
 * \brief Interrupt handler for RS232 RX
 */
void RS232DeviceImpl::rx_irq_handler(){}

/**
 * \brief Interrupt handler for RS232 (DMA ready)
 */
bool RS232DeviceImpl::rx_dma_handler_completed(){
  return false;
}

/**
 * \brief Interrupt handler for RS232 TX (DMA done)
 */
bool RS232DeviceImpl::tx_dma_handler(){
  return false;
}

/**
 * \brief Setup DMA based reception of a string
 *
 * @param buffer The destination where to store the bytes
 * @param length The number of bytes to receive
 * \note This method enables DMA Interrupt after complete reception! (\see rx_dma_handler)
 */
void RS232DeviceImpl::config_dma_rx_irq(uint8_t* buffer, uint32_t length){}

/**
 * \brief Reset status bit and return received byte
 * @param dest Reference where to store the received byte
 * @return true if byte was received, false on error
 * \note this should be called within an interrupt handler
 */
bool RS232DeviceImpl::getc_irq(uint8_t& dest){
  return false;
}


