/*====================================================================
* Function: Serial interface abstraction for TriCore TC1796 SSC
* Description:
*
* Copyright I4Copter Project 2007-2010
* http://www4.informatik.uni-erlangen.de/Research/I4Copter
*=====================================================================
* Author: Peter Ulbrich, Adriaan Schmid, Christian Franzen
*=====================================================================
* $Id: RS232.h 3073 2010-08-23 14:31:18Z sijesche $
*====================================================================*/
#ifndef RS232_H_INCLUDED
#define RS232_H_INCLUDED

#include "GPIO.h"
#include "../system/SystemFrequency.h"

#include <machine/wdtcon.h>

#include <tc1796b/p5.h>
#include <tc1796b/asc0.h>
#include <tc1796b/asc1.h>
#include <tc1796b/asc0-struct.h>
#include <tc1796b/asc1-struct.h>

#define ASC0_ID 0
#define ASC1_ID 1

#define TD_RS232 template<uint8_t _ASC_ID>
#define PL_RS232 RS232<_ASC_ID>

TD_RS232 class RS232
{
    public:
        /**
         * \brief Enable ASC0/1 units
         * \warning This function also configures pins of port 5 / port 6
         * \param baudrate Baudrate to be set
         */
        static bool init(unsigned long baudrate, bool enableRX = true);

        /**
         * \brief Enable hardware stop bit error overrun detection
         * \warning Indication registers must be cleared by software (e.g. check function)
         */
        static bool enableErrorDetection();

        /**
         * \brief Diable hardware stop bit error overrun detection
         */
        static bool disableErrorDetection();

        /**
         * \brief Receive a character from the serial line
         * \param c Place to store the character
         * \return false: no character available, true: OK
         */
        static bool getChar(char *c);

        /**
         * \brief Send a character over the serial line
         * \param c Character to send
         */
        static void putChar(char c);

        /**
         * \brief Enable interrupts of ASC
         * \param TIR_srpn Service request priority number of transmit interrupt (0 to disable)
         * \param TBIR_srpn Service request priority number of transmit buffer interrupt (0 to disable)
         * \param RIR_srpn Service request priority number of receive interrupt (0 to disable)
         * \param EIR_srpn Service request priority number of error interrupt (0 to disable)
         */
        static void enableInterrupts(unsigned char TIR_srpn, unsigned char TBIR_srpn, unsigned char RIR_srpn, unsigned char EIR_srpn);

        /**
         * \brief Receive a character from the serial line in an interrupt handler (without checks)
         * \return The received character
         */
        static char handlerGetChar();

        /**
         * \brief Send a character over the serial line in an interrupt handler (without checks)
         * \param c Character to send
         */
        static void handlerPutChar(char c);

        /**
         * \brief Check the RX error service request an indication registers of ASC
         * \brief This function will also clear the respective SR registers
         * \return The error code (0 no error, 1 frame error, 4 overrun error, 5 both)
         * \warning Hardware error detection has to enabled before using!
         */
        static int handlerCheckRXErrorState();

        static bool enableRx();


	static unsigned int get_rbuf(){
		if(_ASC_ID == 0)
			return (unsigned int) &(m_pASC0->RBUF.reg);
		else
			return (unsigned int) &(m_pASC1->RBUF.reg);
	};


    private:
        static ASC1_t* const m_pASC1;
        static ASC0_t* const m_pASC0;
};



TD_RS232 ASC1_t* const PL_RS232::m_pASC1 = (ASC1_t*) ASC1_BASE;
TD_RS232 ASC0_t* const PL_RS232::m_pASC0 = (ASC0_t*) ASC0_BASE;


TD_RS232 bool PL_RS232::init(unsigned long baudrate, bool enableRX)
{
    unsigned long frequency, reload_value, fdv, dfreq;

    ASCn_CON_t *asc_con;
    ASCn_BG_t *asc_bg;
    ASCn_FDV_t *asc_fdv;
    ASCn_TSRC_t *asc_tsrc;
    ASCn_TBSRC_t *asc_tbsrc;

    // enable ASC
    ::unlock_wdtcon();
    ASC0_CLC.bits.RMC = 1;
    ASC0_CLC.bits.DISR = 0;
    ::lock_wdtcon();

    // calculate frequency settings
    frequency = SystemFrequency::Instance().sysFrequency();
    reload_value = (frequency / (baudrate * 16)) - 1;
    dfreq = frequency / (16*512);
    fdv = (reload_value + 1) * baudrate / dfreq;

    switch (_ASC_ID)
    {
    case ASC0_ID:
        // configure port pins
        ASC0_PISEL.reg = 0;
        GPIO::setFunction(5, 0, GPIO_FUNCTION_INPUT_PULLUP);
        GPIO::setFunction(5, 1, GPIO_FUNCTION_ASC0);
        GPIO::setOutput(5, 1, 1);
        asc_con = &m_pASC0->CON;
        asc_bg = &m_pASC0->BG;
        asc_fdv = &m_pASC0->FDV;
        asc_tsrc = &m_pASC0->TSRC;
        asc_tbsrc = &m_pASC0->TBSRC;
        break;

    case ASC1_ID:
        // configure port pins
        ASC1_PISEL.reg = 1;
        GPIO::setFunction(6, 10, GPIO_FUNCTION_INPUT_PULLUP);
        GPIO::setFunction(6, 11, GPIO_FUNCTION_ASC1);
        GPIO::setOutput(6, 11, 1);
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
    asc_con->bits.R = 1;
    if ( enableRX ) {
        asc_con->bits.REN = 1;
    }
    asc_con->bits.FDE = 1;

    // start tx
    asc_tsrc->bits.SETR = 1;
    asc_tbsrc->bits.SETR = 1;

    return true;
}

TD_RS232 bool PL_RS232::enableRx() {
    ASCn_CON_t *asc_con;

    switch (_ASC_ID)
    {
    case ASC0_ID:
        asc_con = &m_pASC0->CON;
        break;

    case ASC1_ID:
        asc_con = &m_pASC1->CON;
        break;

    default:
        return false;
    }

    asc_con->bits.REN = 1;
    return true;
}

TD_RS232 bool PL_RS232::enableErrorDetection()
{
    ASCn_CON_t *asc_con;

    switch (_ASC_ID)
    {
    case ASC0_ID:
        asc_con = &m_pASC0->CON;
        break;
    case ASC1_ID:
        asc_con = &m_pASC1->CON;
        break;
    default:
        return false;
    }

    asc_con->bits.FEN = 1; // Enable frame error detection
    asc_con->bits.PEN = 1; // Enable parity error detection
    asc_con->bits.OEN = 1; // Enable overrun detection

    return true;
}


TD_RS232 bool PL_RS232::disableErrorDetection()
{
    ASCn_CON_t *asc_con;

    switch (_ASC_ID)
    {
    case ASC0_ID:
        asc_con = &m_pASC0->CON;
        break;
    case ASC1_ID:
        asc_con = &m_pASC1->CON;
        break;
    default:
        return false;
    }

    asc_con->bits.FEN = 0; // Disable frame error detection
    asc_con->bits.PEN = 0; // Disable parity error detection
    asc_con->bits.OEN = 0; // Disable overrun detection

    return true;
}

TD_RS232 bool PL_RS232::getChar(char *c)
{
    switch (_ASC_ID)
    {
    case ASC0_ID:
        if (ASC0_RSRC.bits.SRR != 0)
        {
            *c = (unsigned char) ASC0_RBUF.bits.RD_VALUE;
            ASC0_RSRC.bits.CLRR = 1;
            return true;
        }
        break;
    case ASC1_ID:
        if (ASC1_RSRC.bits.SRR != 0)
        {
            *c = (unsigned char) ASC1_RBUF.bits.RD_VALUE;
            ASC1_RSRC.bits.CLRR = 1;
            return true;
        }
        break;
    default:
        return false;
    }
    return false;
}


TD_RS232 void PL_RS232::putChar(char c)
{
    switch (_ASC_ID)
    {
    case ASC0_ID:
        // Clear transmit buffer request flag
        ASC0_TBSRC.bits.CLRR = 1;
        // Move character to buffer
        ASC0_TBUF.bits.TD_VALUE = c;
        // Wait for transmition completed
        while (ASC0_TSRC.bits.SRR == 0)
            ;
        // Clear transmit request flag
        ASC0_TSRC.bits.CLRR = 1;

        break;
    case ASC1_ID:
        ASC1_TBSRC.bits.CLRR = 1;
        ASC1_TBUF.bits.TD_VALUE = c;
        while (ASC1_TSRC.bits.SRR == 0)
            ;
        ASC1_TSRC.bits.CLRR = 1;

        break;
    default:
        return;
    }
}


TD_RS232 void PL_RS232::enableInterrupts(unsigned char TIR_srpn, unsigned char TBIR_srpn, unsigned char RIR_srpn, unsigned char EIR_srpn)
{
    ASCn_TSRC_t *asc_tsrc;
    ASCn_TBSRC_t *asc_tbsrc;
    ASCn_RSRC_t *asc_rsrc;
    ASCn_ESRC_t *asc_esrc;

    switch (_ASC_ID)
    {
    case ASC0_ID:
        asc_tsrc = &m_pASC0->TSRC;
        asc_tbsrc = &m_pASC0->TBSRC;
        asc_rsrc = &m_pASC0->RSRC;
        asc_esrc = &m_pASC0->ESRC;
        break;
    case ASC1_ID:
        asc_tsrc = &m_pASC1->TSRC;
        asc_tbsrc = &m_pASC1->TBSRC;
        asc_rsrc = &m_pASC1->RSRC;
        asc_esrc = &m_pASC1->ESRC;
        break;
    default:
        return;
    }

    if (TIR_srpn > 0)
    {
        asc_tsrc->bits.SRPN = TIR_srpn;
        asc_tsrc->bits.TOS = 0;
        asc_tsrc->bits.SRE = 1;
    }
    if (TBIR_srpn > 0)
    {
        asc_tbsrc->bits.SRPN = TBIR_srpn;
        asc_tbsrc->bits.TOS = 0;
        asc_tbsrc->bits.SRE = 1;
    }
    if (RIR_srpn > 0)
    {
        asc_rsrc->bits.SRPN = RIR_srpn;
        asc_rsrc->bits.TOS = 0;
        asc_rsrc->bits.SRE = 1;
    }
    if (EIR_srpn > 0)
    {
        asc_esrc->bits.SRPN = EIR_srpn;
        asc_esrc->bits.TOS = 0;
        asc_esrc->bits.SRE = 1;
    }
}


TD_RS232 char PL_RS232::handlerGetChar()
{
    switch (_ASC_ID)
    {
    case ASC0_ID:
        return (unsigned char) ASC0_RBUF.bits.RD_VALUE;
    case ASC1_ID:
        return (unsigned char) ASC1_RBUF.bits.RD_VALUE;
    default:
        return 0;
    }
}


TD_RS232 void PL_RS232::handlerPutChar(char c)
{
    switch (_ASC_ID)
    {
    case ASC0_ID:
        ASC0_TBUF.bits.TD_VALUE = c;
        break;
    case ASC1_ID:
        ASC1_TBUF.bits.TD_VALUE = c;
        break;
    default:
        break;
    }
}


TD_RS232 int PL_RS232::handlerCheckRXErrorState()
{
    int returnValue = 0;

    switch (_ASC_ID)
    {
    case ASC0_ID:
        if (ASC0_ESRC.bits.SRR)
        {
            if (ASC0_CON.bits.FE)          // if framing error
            {
                ASC0_WHBCON.bits.CLRFE = 1;
                returnValue |= 0x01;
            }
            if (ASC0_CON.bits.PE)          // if parity error
            {
                ASC0_WHBCON.bits.CLRPE = 1;
                returnValue |= 0x02;
            }
            if (ASC0_CON.bits.OE)          // if overrun error
            {
                ASC0_WHBCON.bits.CLROE = 1;
                returnValue |= 0x04;
            }

            ASC0_ESRC.bits.CLRR = 1;
        }
        break;

    case ASC1_ID:
        if (ASC1_ESRC.bits.SRR)
        {
            if (ASC1_CON.bits.FE)          // if framing error
            {
                ASC1_WHBCON.bits.CLRFE = 1;
                returnValue |= 0x01;
            }
            if (ASC1_CON.bits.PE)          // if parity error
            {
                ASC1_WHBCON.bits.CLRPE = 1;
                returnValue |= 0x02;
            }
            if (ASC1_CON.bits.OE)          // if overrun error
            {
                ASC1_WHBCON.bits.CLROE = 1;
                returnValue |= 0x04;
            }
            ASC1_ESRC.bits.CLRR = 1;
        }
        break;
    }

    return(returnValue);
}

#endif // RS232_H_INCLUDED
