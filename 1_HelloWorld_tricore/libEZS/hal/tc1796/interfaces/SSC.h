/*====================================================================
* Function: Basic abstraction for TriCore TC1796 SSC
* Description:
*
* Copyright I4Copter Project 2007-2010
* http://www4.informatik.uni-erlangen.de/Research/I4Copter
*=====================================================================
* Author: Peter Ulbrich, Adriaan Schmid, Christian Franzen
*=====================================================================
* $Id: SSC.h 4481 2011-08-10 18:34:51Z ulbrich $
*====================================================================*/
#ifndef SSC_H_INCLUDED
#define SSC_H_INCLUDED

#include "GPIO.h"

#include <stdint.h>
#include <tc1796b/ssc0-struct.h>
#include <tc1796b/ssc1-struct.h>
#include <machine/wdtcon.h>

#include "OSAbstraction.h"
#include "TricoreHardware/system/SystemFrequency.h"

/// SSC units
#define SSC0  0
#define SSC1  1

/// SSC master/slave mode
enum sscMode_t
{
    SSC_MODE_SLAVE = 0,
    SSC_MODE_MASTER = 1
};

/// SSC heading bit -> receive/transmit byte order
enum sscHeadingBit_t
{
    SSC_HEADING_LSB = 0,
    SSC_HEADING_MSB = 1
};

/// SSC clock phase control
enum sscClockPhase_t
{
    SSC_PHASE_TRANSMIT = 0,
    SSC_PHASE_RECEIVE = 1
};

/// SSC clock polarity
enum sscClockPolarity_t
{
    SSC_POLARITY_LOW = 0,
    SSC_POLARITY_HIGH = 1
};

/// SSC slave select output lines
enum sscSlaveSelectLine_t
{
    SSC_SLSO2 = 2,
    SSC_SLSO3 = 3,
    SSC_SLSO4 = 4,
    SSC_SLSO5 = 5,
    SSC_SLSO6 = 6,
    SSC_SLSO7 = 7
};

/// SSC slave select level during activity
enum sscSlaveSelectLevel_t
{
    SSC_SSL_LOW = 0,
    SSC_SSL_HIGH = 1
};

/// SSC slave select input mode
enum sscSlaveSelectInput_t
{
    SSC_SLSI_OFF = 0,
    SSC_SLSI_ON  = 1
};


#define TD_SSC template<uint8_t _SSC_ID>
#define PL_SSC SSC<_SSC_ID>

TD_SSC class SSC
{
public:
    /**
     * \brief Initialise an SSC module
     * \param frequency Base frequency of SSC module
     */
    static void init(unsigned long frequency);

    /**
     * \brief Enable SSC Interrupts
     * \param TSRC_srpn Transmit Interrupt Control Register service request number
     * \param RSRC_srpn Receive Interrupt Control Register service request number
     * \param ESRC_srpn Error Interrupt Control Register service request number
     * \warning Note that this function can only be used with SSC0 module
     */
    static void enableInterrupts(uint8_t TSRC_srpn, uint8_t RSRC_srpn, uint8_t ESRC_srpn);

    /**
     * \brief Reads the SSC Error flags from the status register (TX, RX, Phase, Baudrate Error)
     * \warning Note that this function can only be used with SSC0 module
     */
    static uint8_t getErrorFlags();

    /**
     * \brief Clears the SSC0 Error flags from the status register (TX, RX, Phase, Baudrate Error)
     * \warning Note that this function can only be used with SSC0 module
     */
    static void clearErrorFlags();

    /**
     * \brief Enable error checking flags
     * \warning Note that this function can only be used with SSC0 module
     */
    static void enableErrorChecking();

    /**
     * \brief Setup SSC module communication ports and mode
     * \param mode Communication mode (Master or Slave)
     * \warning Note that in case of the SSC0 module the ports are hard wired and therefore just the mode is set.
     */
    static void setupPorts(sscMode_t mode);

    /**
     * \brief Setup SSC0 RX fifo interrupt level
     * \param level
     * \warning Note that this function can only be used with SSC0 module
     */
    static void setRXFifoLevel(uint8_t level);

    /**
     * \brief Setup SSC module communication baudrate
     * \return Upon success real baudrate set
     * \warning Note that the range and the actual value are depending on the modules clock frequency!
     */
    static int32_t setBaudrate(int32_t baud_rate);

    /**
     * \brief Calculate the value of the br_value register out of the given braudrate in Bit/s
     * \return 16bit register value
     */
    static uint16_t calcBrvalue(int32_t baud_rate);

    /**
     * \brief Sets the value of the br_value register
     */
    static void setBrvalue(uint16_t value);

    /**
     * \brief Enable bus communication for selcted SSC module
     * \warning Note that the bus has to be configured befor enabling it
     */
    static void enableBus();

    /**
     * \brief Disable bus communication for selcted SSC module
     */
    static void disableBus();

    /**
     * \brief Setup SSC module communication configuration
     * \param data_width Bits per communication word (2-16)
     * \param heading_bit Receive/transmit byte order (LSB or MSB)
     * \param clock_polarity Clock polarity in idle mode (LOW or HIGH)
     * \param clock_phase Action on leading clock edge (TRANSMIT or RECEIVE)
     */
    static void setupConfiguration(uint8_t data_width, sscHeadingBit_t heading_bit, sscClockPolarity_t clock_polarity, sscClockPhase_t clock_phase);

    /**
     * \brief Setup SSC bus clock polarity
     * \param clock_polarity Clock polarity in idle mode (LOW or HIGH)
     */
    static void setClockPolarity(sscClockPolarity_t clock_polarity);

    /**
     * \brief Setup SSC bus clock polarity
     * \param clock_phase Action on leading clock edge (TRANSMIT or RECEIVE)
     */
    static void setClockPhase(sscClockPhase_t clock_phase);

    /**
     * \brief Setup SSC bus heading bit
     * \param heading_bit Receive/transmit byte order (LSB or MSB)
     */
    static void setHeading(sscHeadingBit_t heading_bit);

    /**
     * \brief Setup output (slave) communication timing and delays
     * \param leading_delay Leading delay clock cycles (0..3)
     * \param trailing_delay Trailing delay clock cycles (0..3)
     * \param inactive_delay Inactive delay clock cycles (0..3)
     * \warning Note: master mode only, clock cycles length depends on baudrate
     */
    static void setupSlaveTiming(uint8_t leading_delay, uint8_t trailing_delay, uint8_t inactive_delay);

    /**
     * \brief Setup slave select lines
     * \param slave_select Port number of the slave select line (SLSO[2..7])
     * \param slave_select_level Port polarity during inactivity (LOW or HIGH)
     * \warning Note: Slave select line 1 (SLSO1) is hard wired, SLSO1 and SLSO7 have special sematics
     */
    static void setupSlaveSelect(sscSlaveSelectLine_t slave_select, sscSlaveSelectLevel_t slave_select_level);

    /**
     * \brief Select slave to be communicated
     * \param slave_select Port number of the slave select line (SLSO[2..7])
     * \warning Note: Slave select is deselected automaticaly after transmition
     */
    static void selectSlave(sscSlaveSelectLine_t slave_select);

    /**
     * \brief Force deselection of slave select
     * \param slave_select Port number of the slave select line (SLSO[2..7])
     * \warning Note: In case of transmission abortion, use with caution
     */
    //static void unselectSlave(sscSlaveSelectLine slave_select); TODO: Not implemented until now!

    /**
     * \brief Setup slave select input mode
     * \param slave_select_mode Slave select mode (SLSI ON/OFF)
     * \warning Note: SLSI0/1 are hard wired to SSC0/1
     */
    static void setupSlaveSelectInput(sscSlaveSelectInput_t slave_select_mode);

    /**
     * \brief Enable SSC0 FIFO
     * \warning Note: Only the SSC0 module provides a transmit/receive FIFO buffer. The FIFO is set to transparent mode!
     */
    static void enableFifo();

    /**
     * \brief Disable SSC0 FIFO
     * \warning Note: Only the SSC0 module provides a transmit/receive FIFO buffer.
     */
    static void disableFifo();

    /**
     * \brief Send data on selected SSC module
     * \param data Data to be send (current data width)
     * \warning Note: The data length depends on the configured data width (2-16bit)
     * \warning Note: The data has the be right-aligned, unnecessary bits are ignored
     * \warning Note: The byte order is handled by the shift logic
     */
    static void sendData(uint16_t data);

    /**
     * \brief Receive data from selected SSC module
     * \return Content of the receive buffer (current data width)
     * \warning Note: The data length depends on the configured data width (2-16bit)
     * \warning Note: The data is right-aligned, unnecessary bits must be ignored
     * \warning Note: The byte order was handled by the shift logic
     */
    static uint16_t receiveData();

    /**
     * \brief Get the filling level of the SSC0 receive FIFO
     * \return Number of elements in the receive FIFO buffer
     * \return Number of elements in the receive FIFO buffer
     */
    static uint8_t getRXFifoLevel();

    /**
     * \brief Get the filling level of the SSC0 transmit FIFO
     * \return Number of elements in the transmit FIFO buffer
     */
    static uint8_t getTXFifoLevel();

    /**
     * \brief Flush the SSC0 receive FIFO
     */
    static void flushRXFifo();

    /**
     * \brief Flush the SSC0 transmit FIFO
     */
    static void flushTXFifo();

    /**
     * \brief Get actual baudrate
     * \return Baudrate
     */
    static int32_t getBaudrate();

    /**
     * \brief Get bus status (busy/idle)
     * \return Bus state (0 = idle, 1 = busy)
     */
    static uint8_t getBusState();


    /**
     * \brief Get RX address
     * \return RX address
     */
    static uint32_t* getRxAddress();


private:
    static SSC1_t* const m_pSSC1;
    static SSC0_t* const m_pSSC0;

    static uint32_t m_iFreqSys;
    static uint32_t m_iFreqSSC;
};


TD_SSC SSC1_t* const PL_SSC::m_pSSC1 = (SSC1_t*) SSC1_BASE;
TD_SSC SSC0_t* const PL_SSC::m_pSSC0 = (SSC0_t*) SSC0_BASE;
TD_SSC uint32_t PL_SSC::m_iFreqSys  = 0;
TD_SSC uint32_t PL_SSC::m_iFreqSSC  = 0;


TD_SSC void PL_SSC::init(unsigned long frequency)
{
    m_iFreqSys = SystemFrequency::Instance().sysFrequency();
    m_iFreqSSC = frequency;

    switch (_SSC_ID)
    {
    case SSC0:
        ::unlock_wdtcon();
        m_pSSC0->CLC.bits.DISR =  0;  // enable module
        while ((m_pSSC0->CLC.reg & 0x00000002)==2); // wait until module is enabled
        m_pSSC0->FDR.bits.DM = 1; // normal divider mode
        m_pSSC0->FDR.bits.STEP = 1024 - m_iFreqSys / frequency;
        ::lock_wdtcon();

        disableFifo();
        break;

    case SSC1:
        ::unlock_wdtcon();
        m_pSSC1->CLC.bits.DISR =  0;  // enable module
        while ((m_pSSC1->CLC.reg & 0x00000002)==2); // wait until module is enabled
        m_pSSC1->FDR.bits.DM = 1; // normal divider mode
        m_pSSC1->FDR.bits.STEP = 1024 - m_iFreqSys / frequency;
        ::lock_wdtcon();
        break;

    default:
        osPanic();
    }
}


TD_SSC void PL_SSC::enableInterrupts(uint8_t TSRC_srpn, uint8_t RSRC_srpn, uint8_t ESRC_srpn)
{
    // Perform only for SSC0
    if (_SSC_ID == SSC0)
    {
        if (TSRC_srpn > 0)
        {
            m_pSSC0->TSRC.bits.SRPN = TSRC_srpn;
            m_pSSC0->TSRC.bits.TOS = 0;
            m_pSSC0->TSRC.bits.SRE = 1;
        }

        if (RSRC_srpn > 0)
        {
            m_pSSC0->RSRC.bits.SRPN = RSRC_srpn;
            m_pSSC0->RSRC.bits.TOS = 0;
            m_pSSC0->RSRC.bits.SRE = 1;
        }

        if (ESRC_srpn > 0)
        {
            m_pSSC0->ESRC.bits.SRPN = ESRC_srpn;
            m_pSSC0->ESRC.bits.TOS = 0;
            m_pSSC0->ESRC.bits.SRE = 1;
        }
    }
    else
    {
        if (TSRC_srpn > 0)
        {
            m_pSSC1->TSRC.bits.SRPN = TSRC_srpn;
            m_pSSC1->TSRC.bits.TOS = 0;
            m_pSSC1->TSRC.bits.SRE = 1;
        }

        if (RSRC_srpn > 0)
        {
            m_pSSC1->RSRC.bits.SRPN = RSRC_srpn;
            m_pSSC1->RSRC.bits.TOS = 0;
            m_pSSC1->RSRC.bits.SRE = 1;
        }

        if (ESRC_srpn > 0)
        {
            m_pSSC1->ESRC.bits.SRPN = ESRC_srpn;
            m_pSSC1->ESRC.bits.TOS = 0;
            m_pSSC1->ESRC.bits.SRE = 1;
        }
    }
}


TD_SSC uint8_t PL_SSC::getErrorFlags()
{
    if (_SSC_ID == SSC0)
    {
        return (((m_pSSC0->STAT.reg) & 0xF00) >> 8);

    }
    else
    {
        return (((m_pSSC1->STAT.reg) & 0xF00) >> 8);
    }
}


TD_SSC void PL_SSC::clearErrorFlags()
{
    if (_SSC_ID == SSC0)
    {
        /*
         m_pSSC0->EFM.bits.CLRTE = 1;
         m_pSSC0->EFM.bits.CLRRE = 1;
         m_pSSC0->EFM.bits.CLRPE = 1;
         m_pSSC0->EFM.bits.CLRBE = 1;
         // short:
         */
        m_pSSC0->EFM.reg |= 0xF00;
    }
    else
    {
        m_pSSC1->EFM.reg |= 0xF00;
    }
}


TD_SSC void PL_SSC::enableErrorChecking()
{
    // Perform only for SSC0
    if (_SSC_ID == SSC0)
    {
        m_pSSC0->CON.bits.BEN = 1;
        m_pSSC0->CON.bits.PEN = 1;
        m_pSSC0->CON.bits.REN = 1;
        m_pSSC0->CON.bits.TEN = 1;
    }
    else
    {
        m_pSSC1->CON.bits.BEN = 1;
        m_pSSC1->CON.bits.PEN = 1;
        m_pSSC1->CON.bits.REN = 1;
        m_pSSC1->CON.bits.TEN = 1;
    }
}


TD_SSC void PL_SSC::setupPorts(sscMode_t mode)
{
    switch (_SSC_ID)
    {
    case SSC0:
        m_pSSC0->CON.bits.MS = mode;
        break;

    case SSC1:
        if (mode == SSC_MODE_SLAVE)
        {
            //  P6.6 is used as SSC1 clock input signal (SSC1_SCLK1)
            GPIO::setFunction(6, 6, GPIO_FUNCTION_INPUT_PULLUP);
            //  P6.4 is used as SSC1 master transmit slave receive input signal (SSC1_MTSR1)
            GPIO::setFunction(6, 4, GPIO_FUNCTION_INPUT_PULLUP);
            //  P6.5 is used as SSC1 master receive slave transmit output signal (SSC1_MRST1)
            GPIO::setFunction(6, 5, GPIO_FUNCTION_SSC);
            m_pSSC1->PISEL.bits.SLSIS = 1;
        }
        else
        {
            // TODO: Untested!!
            //  P6.6 is used as SSC1 clock output signal (SSC1_SCLK1)
            GPIO::setFunction(6, 6, GPIO_FUNCTION_SSC);
            //  P6.4 is used as SSC1 master transmit slave receive output signal (SSC1_MTSR1)
            GPIO::setFunction(6, 4, GPIO_FUNCTION_SSC);
            //  P6.5 is used as SSC1 master receive slave transmit input signal (SSC1_MRST1)
            GPIO::setFunction(6, 5, GPIO_FUNCTION_INPUT_PULLDOWN);
            m_pSSC1->PISEL.bits.SLSIS = 1;
        }
        m_pSSC1->CON.bits.MS = mode;
        break;

    default:
        osPanic();
    }
}


TD_SSC int32_t PL_SSC::setBaudrate(int32_t baud_rate)
{
    if (!m_iFreqSys) return -1;

    switch (_SSC_ID)
    {
    case SSC0:
        m_pSSC0->BR.bits.BR_VALUE = (m_iFreqSys / (2 * baud_rate * (1024 - m_pSSC0->FDR.bits.STEP))) - 1;
        return m_pSSC0->BR.bits.BR_VALUE;

    case SSC1:
        m_pSSC1->BR.bits.BR_VALUE = (m_iFreqSys / (2 * baud_rate * (1024 - m_pSSC1->FDR.bits.STEP))) - 1;
        return m_pSSC1->BR.bits.BR_VALUE;

    default:
        osPanic();
    }
}

TD_SSC uint16_t PL_SSC::calcBrvalue(int32_t baud_rate)
{
    switch (_SSC_ID)
    {
    case SSC0:
        return (m_iFreqSys / (2 * baud_rate * (1024 - m_pSSC0->FDR.bits.STEP))) - 1;

    case SSC1:
        return (m_iFreqSys / (2 * baud_rate * (1024 - m_pSSC1->FDR.bits.STEP))) - 1;

    default:
        osPanic();
    }
}


TD_SSC void PL_SSC::setBrvalue(uint16_t value)
{
    switch (_SSC_ID)
    {
    case SSC0:
        m_pSSC0->BR.bits.BR_VALUE = value;
        break;

    case SSC1:
        m_pSSC1->BR.bits.BR_VALUE = value;
        break;

    default:
        osPanic();
    }
}


TD_SSC void PL_SSC::enableBus()
{
    SSCn_CON_t* pCON;

    switch (_SSC_ID)
    {
    case SSC0:
        pCON = &(m_pSSC0->CON);
        break;

    case SSC1:
        pCON = &(m_pSSC1->CON);
        break;

    default:
        osPanic();
    }

    while (getBusState() != 0) {};

    do
    {
        pCON->bits.EN = 1;
    }
    while (pCON->bits.EN != 1);
}


TD_SSC void PL_SSC::disableBus()
{
    SSCn_CON_t* pCON;

    switch (_SSC_ID)
    {
    case SSC0:
        pCON = &(m_pSSC0->CON);
        break;

    case SSC1:
        pCON = &(m_pSSC1->CON);
        break;

    default:
        osPanic();
    }

    do
    {
        pCON->bits.EN = 0;
    }
    while (pCON->bits.EN != 0);
}


TD_SSC void PL_SSC::setupConfiguration(uint8_t data_width, sscHeadingBit_t heading_bit, sscClockPolarity_t clock_polarity, sscClockPhase_t clock_phase)
{
    SSCn_CON_t* pCON;

    switch (_SSC_ID)
    {
    case SSC0:
        pCON = &(m_pSSC0->CON);
        break;

    case SSC1:
        pCON = &(m_pSSC1->CON);
        break;

    default:
        osPanic();
    }

    // Set data width
    if ((data_width <= 16) && (data_width > 0))
    {
        pCON->bits.BM = data_width - 1;
    }

    // Set heading bit control
    pCON->bits.HB = heading_bit;

    // Set clock polarity control
    pCON->bits.PO = clock_polarity;

    // Set clock phase control
    pCON->bits.PH = clock_phase;
}


TD_SSC void PL_SSC::setClockPolarity(sscClockPolarity_t clock_polarity)
{
    switch (_SSC_ID)
    {
    case SSC0:
        m_pSSC0->CON.bits.PO = clock_polarity;
        break;

    case SSC1:
        m_pSSC1->CON.bits.PO = clock_polarity;
        break;

    default:
        osPanic();
    }
}


TD_SSC void PL_SSC::setClockPhase(sscClockPhase_t clock_phase)
{
    switch (_SSC_ID)
    {
    case SSC0:
        m_pSSC0->CON.bits.PH = clock_phase;
        break;

    case SSC1:
        m_pSSC1->CON.bits.PH = clock_phase;
        break;

    default:
        osPanic();
    }
}


TD_SSC void PL_SSC::setHeading(sscHeadingBit_t heading_bit)
{
    switch (_SSC_ID)
    {
    case SSC0:
        m_pSSC0->CON.bits.HB = heading_bit;
        break;

    case SSC1:
        m_pSSC1->CON.bits.HB = heading_bit;
        break;

    default:
        osPanic();
    }
}


TD_SSC void PL_SSC::setupSlaveTiming(uint8_t leading_delay, uint8_t trailing_delay, uint8_t inactive_delay)
{
    SSCn_SSOTC_t* pSSOTC;

    switch (_SSC_ID)
    {
    case SSC0:
        pSSOTC = &(m_pSSC0->SSOTC);
        break;

    case SSC1:
        pSSOTC = &(m_pSSC1->SSOTC);
        break;

    default:
        osPanic();
    }

    pSSOTC->bits.LEAD = leading_delay;
    pSSOTC->bits.TRAIL = trailing_delay;
    pSSOTC->bits.INACT = inactive_delay;
}


TD_SSC void PL_SSC::setupSlaveSelect(sscSlaveSelectLine_t slave_select, sscSlaveSelectLevel_t slave_select_level)
{
    SSCn_SSOC_t* pSSOC;

    switch (_SSC_ID)
    {
    case SSC0:
        GPIO::setFunction(2, slave_select, GPIO_FUNCTION_SSC0); // Connect respective pin (Port 2.x) to ssc 0 module
        pSSOC = &(m_pSSC0->SSOC);
        break;

    case SSC1:
        GPIO::setFunction(2, slave_select, GPIO_FUNCTION_SSC1); // Connect respective pin (Port 2.x) to ssc 1 module
        pSSOC = &(m_pSSC1->SSOC);
        break;

    default:
        osPanic();
    }

    pSSOC->reg &= ~0x0000ff00; // Reset slave enable bits

    switch (slave_select_level)
    {
    case SSC_SSL_HIGH:
        pSSOC->reg |= (1 << slave_select); // Set slave select activity level
        break;
    case SSC_SSL_LOW:
        pSSOC->reg &= ~(1 << slave_select); // Set slave select activity level
        break;
    }
}


TD_SSC void PL_SSC::selectSlave(sscSlaveSelectLine_t slave_select)
{
    switch (_SSC_ID)
    {
    case SSC0:
        m_pSSC0->SSOC.reg &= ~0x0000ff00; // Reset all slave enable bits
        m_pSSC0->SSOC.reg |= (1 << (slave_select + 8)); // Set slave enable bit
        break;

    case SSC1:
        m_pSSC1->SSOC.reg &= ~0x0000ff00; // Reset all slave enable bits
        m_pSSC1->SSOC.reg |= (1 << (slave_select + 8)); // Set slave enable bit
        break;

    default:
        osPanic();
    }
}

TD_SSC void PL_SSC::setupSlaveSelectInput(sscSlaveSelectInput_t slave_select_mode)
{
    switch (_SSC_ID)
    {
    case SSC0:
        m_pSSC0->PISEL.bits.SLSIS = slave_select_mode;
        break;

    case SSC1:
        if (slave_select_mode == SSC_SLSI_ON)
        {
            //  P6.7 is used as SSC1 slave select input signal (SSC1_SLSI1)
            GPIO::setFunction(6, 7, GPIO_FUNCTION_INPUT_PULLUP);
        }
        m_pSSC1->PISEL.bits.SLSIS = slave_select_mode;
        break;

    default:
        osPanic();
    }
}



TD_SSC void PL_SSC::sendData(uint16_t data)
{
    switch (_SSC_ID)
    {
    case SSC0:
        // Wait while TX FIFO is full
        while (m_pSSC0->TXFCON.bits.TXFEN && (m_pSSC0->FSTAT.bits.TXFFL == 0x8));
        m_pSSC0->TSRC.bits.CLRR = 1; // reset transmit interrupt request
        m_pSSC0->TB.bits.TB_VALUE = data; // load transmit buffer register
        break;

    case SSC1:
        m_pSSC1->TSRC.bits.CLRR = 1; // reset transmit interrupt request
        m_pSSC1->TB.bits.TB_VALUE = data; // load transmit buffer register
        break;

    default:
        osPanic();
    }
}


TD_SSC uint16_t PL_SSC::receiveData()
{
    switch (_SSC_ID)
    {
    case SSC0:
        //m_pSSC0->RSRC.bits.CLRR = 1;//  reset receive interrupt request
        return m_pSSC0->RB.bits.RB_VALUE; //  read receive buffer register

    case SSC1:
        m_pSSC1->RSRC.bits.CLRR = 1;//  reset transmit interrupt request
        return m_pSSC1->RB.bits.RB_VALUE; //  read receive buffer register

    default:
        osPanic();
    }
}


TD_SSC void PL_SSC::enableFifo()
{
    // Perform only for SSC0
    if (_SSC_ID == SSC0)
    {
        m_pSSC0->RXFCON.bits.RXFFLU = 1; // Flush RX fifo
        m_pSSC0->TXFCON.bits.TXFFLU = 1; // Flush TX fifo

        m_pSSC0->RXFCON.bits.RXFEN = 1; // Enable RX fifo
        m_pSSC0->TXFCON.bits.TXFEN = 1; // Enable TX fifo
    }
    else
    {
        osPanic();
    }
}


TD_SSC void PL_SSC::disableFifo()
{
    // Perform only for SSC0
    if (_SSC_ID == SSC0)
    {
        m_pSSC0->RXFCON.bits.RXFFLU = 1; // Flush RX fifo
        m_pSSC0->TXFCON.bits.TXFFLU = 1; // Flush TX fifo

        m_pSSC0->RXFCON.bits.RXFEN = 0; // Disable RX fifo
        m_pSSC0->TXFCON.bits.TXFEN = 0; // Disable TX fifo
    }
    else
    {
        osPanic();
    }
}


TD_SSC void PL_SSC::setRXFifoLevel(uint8_t level)
{
    // Perform only for SSC0
    if (_SSC_ID == SSC0)
    {
        m_pSSC0->RXFCON.bits.RXFITL = level;
    }
    else
    {
        osPanic();
    }
}


TD_SSC uint8_t PL_SSC::getRXFifoLevel()
{
    // Perform only for SSC0
    if (_SSC_ID == SSC0)
    {
        return m_pSSC0->FSTAT.bits.RXFFL;
    }
    else
    {
        osPanic();
    }
}


TD_SSC uint8_t PL_SSC::getTXFifoLevel()
{
    // Perform only for SSC0
    if (_SSC_ID == SSC0)
    {
        return m_pSSC0->FSTAT.bits.TXFFL;
    }
    else
    {
        osPanic();
    }
}


TD_SSC void PL_SSC::flushRXFifo()
{
    // Perform only for SSC0
    if (_SSC_ID == SSC0)
    {
        m_pSSC0->RXFCON.bits.RXFFLU = 1;
    }
    else
    {
        osPanic();
    }
}


TD_SSC void PL_SSC::flushTXFifo()
{
    // Perform only for SSC0
    if (_SSC_ID == SSC0)
    {
        m_pSSC0->TXFCON.bits.TXFFLU = 1;
    }
    else
    {
        osPanic();
    }
}


TD_SSC uint8_t PL_SSC::getBusState()
{
    switch (_SSC_ID)
    {
    case SSC0:
        return m_pSSC0->STAT.bits.BSY;

    case SSC1:
        return m_pSSC1->STAT.bits.BSY;

    default:
        osPanic();
    }
}
TD_SSC uint32_t* PL_SSC::getRxAddress()
{
    switch (_SSC_ID)
    {
    case SSC0:
        return (uint32_t *)(&(m_pSSC0->RB.reg));

    case SSC1:
        return (uint32_t *)(&(m_pSSC1->RB.reg));

    default:
        osPanic();
    }
}
#endif // SSC_H_INCLUDED
