/*====================================================================
* Function: Basic abstraction for TriCore TC1796 IOLS
* Description:
*
* Copyright I4Copter Project 2007-2010
* http://www4.informatik.uni-erlangen.de/Research/I4Copter
*=====================================================================
* Author: Peter Ulbrich, Adriaan Schmid, Christian Franzen
*=====================================================================
* $Id: IOLS.h 3156 2010-09-10 07:14:00Z sijesche $
*====================================================================*/
#ifndef IOLS_H_INCLUDED
#define IOLS_H_INCLUDED

#include "OSAbstraction.h"
#include <stdint.h>
#include <tc1796b/gpta0-struct.h>
#include <tc1796b/gpta1-struct.h>

/* get the FPC number from the input pin */
#define IOLS_GET_FPC_FROM_PIN(x) ((x % 12) / 2)

/* get the FPC SIN number from the input pin */
#define IOLS_GET_FPCSIN_FROM_PIN(x) ((x - (x % 12)) / 12)


#define IOLS_DST_CLASS_OUT  0
#define IOLS_DST_CLASS_IO   1
#define IOLS_DST_CLASS_GTC  2
#define IOLS_DST_CLASS_LTC  3

#define IOLS_OUT_SRC_CLASS_GTC      0
#define IOLS_OUT_SRC_CLASS_LTC_03   1
#define IOLS_OUT_SRC_CLASS_LTC_47   2

#define IOLS_IO_SRC_CLASS_GTC       0
#define IOLS_IO_SRC_CLASS_LTC_03    1
#define IOLS_IO_SRC_CLASS_LTC_47    2

#define IOLS_GTC_SRC_CLASS_IO_03    0
#define IOLS_GTC_SRC_CLASS_IO_46    1
#define IOLS_GTC_SRC_CLASS_LTC_03   2
#define IOLS_GTC_SRC_CLASS_LTC_47   3
#define IOLS_GTC_SRC_CLASS_FPC_INT  4

#define IOLS_LTC_SRC_CLASS_IO_03    0
#define IOLS_LTC_SRC_CLASS_IO_46    1
#define IOLS_LTC_SRC_CLASS_GTC      2
#define IOLS_LTC_SRC_CLASS_CLK      3
#define IOLS_LTC_SRC_CLASS_PDL_INT  4



#define TD_IOLS template<uint8_t _GPTA_ID>
#define PL_IOLS IOLS<_GPTA_ID>

TD_IOLS class IOLS
{
    protected:
        static void modifyMRA(unsigned short index, unsigned long cr_mask, unsigned long cr_val);

    public:
        /**
         * \brief Connect the input of an LTC to an IO pin
         * \param ltc_number The number of the LTC to connect
         * \param io_number The number of the IO pin to connect
         * \return true if the required connection is valid, false otherwise
         */
        static bool connectLTCInputIO(uint8_t ltc_number, uint8_t io_number);

        /**
         * \brief Connect the input of an LTC to a GTC output
         * \param ltc_number The number of the LTC to connect
         * \param gtc_number The number of the GTC to connect
         * \return true if the required connection is valid, false otherwise
         */
        static bool connectLTCInputGTC(uint8_t ltc_number, uint8_t gtc_number);

        /**
         * \brief Connect the input of an LTC to a clock bus line
         * \param ltc_number The number of the LTC to connect
         * \param clock_number The number of the clock bus line to connect (0-7)
         * \return true if the required connection is valid, false otherwise
         */
        static bool connectLTCInputClock(uint8_t ltc_number, uint8_t clock_number);

        /**
         * \brief Connect the input of an LTC to a PDL line
         * \param ltc_number The number of the LTC to connect
         * \param pdl_number The number of the PDL to connect (0-3)
         * \return true if the required connection is valid, false otherwise
         */
        static bool connectLTCInputPDL(uint8_t ltc_number, uint8_t pdl_number);

        /**
         * \brief Connect the input of an LTC to an interrupt line
         * \param ltc_number The number of the LTC to connect
         * \param int_number The number of the interrupt line to connect (0-3)
         * \return true if the required connection is valid, false otherwise
         */
        static bool connectLTCInputInt(uint8_t ltc_number, uint8_t int_number);

        /**
         * \brief Connect the input of a GTC to an IO pin
         * \param gtc_number The number of the GTC to connect
         * \param io_number The number of the IO pin to connect
         * \return true if the required connection is valid, false otherwise
         */
        static bool connectGTCInputIO(uint8_t gtc_number, uint8_t io_number);

        /**
         * \brief Connect the input of a GTC to an LTC output
         * \param gtc_number The number of the GTC to connect
         * \param ltc_number The number of the LTC to connect
         * \return true if the required connection is valid, false otherwise
         */
        static bool connectGTCInputLTC(uint8_t gtc_number, uint8_t ltc_number);

        /**
         * \brief Connect the input of a GTC to an FPC
         * \param gtc_number The number of the GTC to connect
         * \param fpc_number The number of the FPC to connect (0-5)
         * \return true if the required connection is valid, false otherwise
         */
        static bool connectGTCInputFPC(uint8_t gtc_number, uint8_t fpc_number);

        /**
         * \brief Connect the input of a GTC to an interrupt line
         * \param gtc_number The number of the GTC to connect
         * \param int_number The number of the interrupt to connect (0-1)
         * \return true if the required connection is valid, false otherwise
         */
        static bool connectGTCInputInt(uint8_t gtc_number, uint8_t int_number);

        /**
         * \brief Connect an output pin to an LTC
         * \param out_number The number of the output pin to connect
         * \param ltc_number The number of the LTC to connect
         * \return true if the required connection is valid, false otherwise
         */
        static bool connectOutLTC(uint8_t out_number, uint8_t ltc_number);

        /**
         * \brief Connect an output pin to a GTC
         * \param out_number The number of the output pin to connect
         * \param gtc_number The number of the GTC to connect
         * \return true if the required connection is valid, false otherwise
         */
        static bool connectOutGTC(uint8_t out_number, uint8_t gtc_number);

        /**
         * \brief Connect an IO pin to an LTC
         * \param io_number The number of the IO pin to connect
         * \param ltc_number The number of the LTC to connect
         * \return true if the required connection is valid, false otherwise
         */
        static bool connectIoLTC(uint8_t io_number, uint8_t ltc_number);

        /**
         * \brief Connect an IO pin to a GTC
         * \param io_number The number of the IO pin to connect
         * \param gtc_number The number of the GTC to connect
         * \return true if the required connection is valid, false otherwise
         */
        static bool connectIoGTC(uint8_t io_number, uint8_t gtc_number);

    private:
        /**
         * \brief The private function to actually connect
         * \param dest_class The destination class (IOLS_DST_CLASS_(OUT|IO|LTC|GTC))
         * \param dest_number Number of the destination device/pin
         * \param src_class The source class (IOLS_(dest_class)_SRC_CLASS_(IO_03|IO_46|LTC_03|LTC_46|GTC|CLK|FDL_INT))
         * \param src_number Number of the source device/pin
         * \param need_enable Whether to set the enable bit (required for GTC and LTC inputs)
         * \warning Note that changes only take effect after calling uploadMRA()
         */
        static void connect(uint8_t dest_class, uint8_t dest_number, uint8_t src_class, uint8_t src_number, bool need_enable);


        static GPTA0_t* const m_pGPTA0;
        static GPTA1_t* const m_pGPTA1;
};

TD_IOLS GPTA0_t* const PL_IOLS::m_pGPTA0 = (GPTA0_t *) GPTA0_BASE;
TD_IOLS GPTA1_t* const PL_IOLS::m_pGPTA1 = (GPTA1_t *) GPTA1_BASE;

TD_IOLS bool PL_IOLS::connectLTCInputIO(uint8_t ltc_number, uint8_t io_number)
{
    /*
     * check if the required combination is valid
     */
    unsigned short ltcg = ltc_number >> 3;
    unsigned short iog = io_number >> 3;

    switch (ltcg)
    {
        case 0: if (iog != 0 && iog != 4) return false; break;
        case 1: if (iog != 1 && iog != 5) return false; break;
        case 2: if (iog != 2 && iog != 6) return false; break;
        case 3: if (iog != 3) return false; break;
        case 4: if (iog != 0 && iog != 4) return false; break;
        case 5: if (iog != 1 && iog != 5) return false; break;
        case 6: if (iog != 2 && iog != 6) return false; break;
        case 7: if (iog != 3) return false; break;
        default: return false;
    }

    if (iog >> 2) {
        // groups 4-6
        connect(IOLS_DST_CLASS_LTC, ltc_number, IOLS_LTC_SRC_CLASS_IO_46, io_number, true);
    } else {
        // groups 0-3
        connect(IOLS_DST_CLASS_LTC, ltc_number, IOLS_LTC_SRC_CLASS_IO_03, io_number, true);
    }

    return true;
}


TD_IOLS bool PL_IOLS::connectLTCInputGTC(uint8_t ltc_number, uint8_t gtc_number)
{
    /*
     * check if the required combination is valid
     */
    unsigned short ltcg = ltc_number >> 3;
    unsigned short gtcg = gtc_number >> 3;

    switch (ltcg)
    {
        case 0: if (gtcg != 0) return false; break;
        case 1: if (gtcg != 1) return false; break;
        case 2: if (gtcg != 2) return false; break;
        case 3: if (gtcg != 3) return false; break;
        case 4: if (gtcg != 0) return false; break;
        case 5: if (gtcg != 1) return false; break;
        case 6: if (gtcg != 2) return false; break;
        case 7: if (gtcg != 3) return false; break;
        default: return false;
    }

    connect(IOLS_DST_CLASS_LTC, ltc_number, IOLS_LTC_SRC_CLASS_GTC, gtc_number, true);
    return true;
}


TD_IOLS bool PL_IOLS::connectLTCInputClock(uint8_t ltc_number, uint8_t clock_number)
{
    connect(IOLS_DST_CLASS_LTC, ltc_number, IOLS_LTC_SRC_CLASS_CLK, clock_number, true);
    return true;
}


TD_IOLS bool PL_IOLS::connectLTCInputPDL(uint8_t ltc_number, uint8_t pdl_number)
{
    connect(IOLS_DST_CLASS_LTC, ltc_number, IOLS_LTC_SRC_CLASS_PDL_INT, pdl_number & 3, true);
    return true;
}


TD_IOLS bool PL_IOLS::connectLTCInputInt(uint8_t ltc_number, uint8_t int_number)
{
    connect(IOLS_DST_CLASS_LTC, ltc_number, IOLS_LTC_SRC_CLASS_PDL_INT, 4 + (int_number & 3), true);
    return true;
}


TD_IOLS bool PL_IOLS::connectGTCInputIO(uint8_t gtc_number, uint8_t io_number)
{
    /*
     * check if the request is valid
     */
    unsigned short gtcg = gtc_number >> 3;
    unsigned short iog = io_number >> 3;

    switch (gtcg)
    {
        case 0: if (iog != 0 && iog != 4) return false; break;
        case 1: if (iog != 1 && iog != 5) return false; break;
        case 2: if (iog != 2 && iog != 6) return false; break;
        case 3: if (iog != 3) return false; break;
        default: return false;
    }

    if (iog >> 2) {
        // io groups 4-6
        connect(IOLS_DST_CLASS_GTC, gtc_number, IOLS_GTC_SRC_CLASS_IO_46, io_number, true);
    } else {
        // io groups 0-3
        connect(IOLS_DST_CLASS_GTC, gtc_number, IOLS_GTC_SRC_CLASS_IO_03, io_number, true);
    }

    return true;
}


TD_IOLS bool PL_IOLS::connectGTCInputLTC(uint8_t gtc_number, uint8_t ltc_number)
{
    /*
     * check if the request is valid
     */
    unsigned short gtcg = gtc_number >> 3;
    unsigned short ltcg = ltc_number >> 3;

    switch (gtcg)
    {
        case 0: if (ltcg != 0 && ltcg != 4) return false; break;
        case 1: if (ltcg != 1 && ltcg != 5) return false; break;
        case 2: if (ltcg != 2 && ltcg != 6) return false; break;
        case 3: if (ltcg != 3 && ltcg != 7) return false; break;
        default: return false;
    }
    if (ltcg >> 2) {
        // ltc groups 4-7
        connect(IOLS_DST_CLASS_GTC, gtc_number, IOLS_GTC_SRC_CLASS_LTC_47, ltc_number, true);
    } else {
        // ltc groups 0-3
        connect(IOLS_DST_CLASS_GTC, gtc_number, IOLS_GTC_SRC_CLASS_LTC_03, ltc_number, true);
    }

    return true;
}


TD_IOLS bool PL_IOLS::connectGTCInputFPC(uint8_t gtc_number, uint8_t fpc_number)
{
    connect(IOLS_DST_CLASS_GTC, gtc_number, IOLS_GTC_SRC_CLASS_FPC_INT, fpc_number, true);
    return true;
}


TD_IOLS bool PL_IOLS::connectGTCInputInt(uint8_t gtc_number, uint8_t int_number)
{
    connect(IOLS_DST_CLASS_GTC, gtc_number, IOLS_GTC_SRC_CLASS_FPC_INT, 6 + int_number, true);
    return true;
}


TD_IOLS bool PL_IOLS::connectOutLTC(uint8_t out_number, uint8_t ltc_number)
{
    /*
     * check if the request is valid
     */
    unsigned short og = out_number >> 3;
    unsigned short ltcg = ltc_number >> 3;

    switch (og)
    {
        case 0: if (ltcg != 3 && ltcg != 7) return false; break;
        case 1: if (ltcg != 0 && ltcg != 4) return false; break;
        case 2: if (ltcg != 1 && ltcg != 5) return false; break;
        case 3: if (ltcg != 2 && ltcg != 6) return false; break;
        case 4: if (ltcg != 3 && ltcg != 7) return false; break;
        case 5: if (ltcg != 0 && ltcg != 4) return false; break;
        case 6: if (ltcg != 1 && ltcg != 5) return false; break;
        default: return false;
    }

    if (ltcg >> 2) {
        // ltc groups 4-7
        connect(IOLS_DST_CLASS_OUT, out_number, IOLS_OUT_SRC_CLASS_LTC_47, ltc_number, false);
    } else {
        // ltc groups 0-3
        connect(IOLS_DST_CLASS_OUT, out_number, IOLS_OUT_SRC_CLASS_LTC_03, ltc_number, false);
    }

    return true;
}


TD_IOLS bool PL_IOLS::connectOutGTC(uint8_t out_number, uint8_t gtc_number)
{
    /*
     * check if the request is valid
     */
    unsigned short og = out_number >> 3;
    unsigned short gtcg = gtc_number >> 3;

    switch (og)
    {
        case 0: if (gtcg != 3) return false; break;
        case 1: if (gtcg != 0) return false; break;
        case 2: if (gtcg != 1) return false; break;
        case 3: if (gtcg != 2) return false; break;
        case 4: if (gtcg != 3) return false; break;
        case 5: if (gtcg != 0) return false; break;
        case 6: if (gtcg != 1) return false; break;
        default: return 1;
    }

    connect(IOLS_DST_CLASS_OUT, out_number, IOLS_OUT_SRC_CLASS_GTC, gtc_number, false);
    return true;
}


TD_IOLS bool PL_IOLS::connectIoLTC(uint8_t io_number, uint8_t ltc_number)
{
    /*
     * check if the request is valid
     */
    unsigned short iog = io_number >> 3;
    unsigned short ltcg = ltc_number >> 3;

    switch (iog)
    {
        case 0: if (ltcg != 0 && ltcg != 4) return false; break;
        case 1: if (ltcg != 1 && ltcg != 5) return false; break;
        case 2: if (ltcg != 2 && ltcg != 6) return false; break;
        case 3: if (ltcg != 3 && ltcg != 7) return false; break;
        case 4: if (ltcg != 0 && ltcg != 4) return false; break;
        case 5: if (ltcg != 1 && ltcg != 5) return false; break;
        case 6: if (ltcg != 2 && ltcg != 6) return false; break;
        default: return false;
    }

    if (ltcg >> 2) {
        // ltc groups 4-7
        connect(IOLS_DST_CLASS_IO, io_number, IOLS_IO_SRC_CLASS_LTC_47, ltc_number, false);
    } else {
        // ltc groups 0-3
        connect(IOLS_DST_CLASS_IO, io_number, IOLS_IO_SRC_CLASS_LTC_03, ltc_number, false);
    }

    return true;
}


TD_IOLS bool PL_IOLS::connectIoGTC(uint8_t io_number, uint8_t gtc_number)
{
    /*
     * check if the request is valid
     */
    unsigned short iog = io_number >> 3;
    unsigned short gtcg = gtc_number >> 3;

    switch (iog)
    {
        case 0: if (gtcg != 0) return false; break;
        case 1: if (gtcg != 1) return false; break;
        case 2: if (gtcg != 2) return false; break;
        case 3: if (gtcg != 3) return false; break;
        case 4: if (gtcg != 0) return false; break;
        case 5: if (gtcg != 1) return false; break;
        case 6: if (gtcg != 2) return false; break;
        default: return false;
    }

    connect(IOLS_DST_CLASS_IO, io_number, IOLS_IO_SRC_CLASS_GTC, gtc_number, false);
    return true;
}


TD_IOLS void PL_IOLS::connect(uint8_t dest_class, uint8_t dest_number, uint8_t src_class, uint8_t src_number, bool need_enable)
{
    unsigned long cr_mask = 0x000000ff << (8 * (dest_number & 3));
    unsigned long cr_val =
        ((need_enable ? 0x00000080 : 0x00000000) |
         (src_class << 4) |
         (src_number & 7))
        << (8 * (dest_number & 3));

    unsigned short index = 0;
    switch (dest_class)
    {
        case IOLS_DST_CLASS_GTC: index = 0; break;
        case IOLS_DST_CLASS_LTC: index = 8; break;
        case IOLS_DST_CLASS_IO: index = 24; break;
        case IOLS_DST_CLASS_OUT: index = 38; break;
    }

    modifyMRA(index + (dest_number >> 2), cr_mask, cr_val);

}

TD_IOLS void PL_IOLS::modifyMRA(unsigned short index, unsigned long cr_mask, unsigned long cr_val)
{
    GPTAn_MRACTL_t* mractl;
    GPTAn_MRADIN_t* mradin;
    GPTAn_MRADOUT_t* mradout;
    unsigned short i;

    switch (_GPTA_ID) {
        case 0:
            mractl = &(m_pGPTA0->MRACTL);
            mradin = &(m_pGPTA0->MRADIN);
            mradout = &(m_pGPTA0->MRADOUT);
            break;
        case 1:
            mractl = &(m_pGPTA1->MRACTL);
            mradin = &(m_pGPTA1->MRADIN);
            mradout = &(m_pGPTA1->MRADOUT);
            break;
        default:
            osPanic();
    }

    unsigned long MRA[52];

    /*
     * Init multiplexer array
     * for some reason this only works if we use MRACTL.reg
     * (and not MRACTL.bits.*)
     */
    mractl->reg = 0; // clear MAEN
    mractl->reg = 2; // set WCRES
    __asm__("dsync");
    __asm__("isync");

    for (i = 0; i < 52; i++) {
        mradin->reg = 0x0;

        MRA[51 - i] = mradout->reg;
        __asm__("dsync");
        __asm__("isync");
    }

    mractl->reg = 2; // set WCRES

    MRA[index] &= ~cr_mask;
    MRA[index] |= cr_val;

    for (i = 0; i < 52; i++)
    {
        mradin->reg = MRA[51 - i];
        __asm__("dsync");
        __asm__("isync");
    }

    mractl->reg = 1; // set MAEN
}

#endif // IOLS_H_INCLUDED
