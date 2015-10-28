/*====================================================================
* Function: Basic abstraction for TriCore TC1796 FPC
* Description:
*
* Copyright I4Copter Project 2007-2010
* http://www4.informatik.uni-erlangen.de/Research/I4Copter
*=====================================================================
* Author: Peter Ulbrich, Adriaan Schmid, Christian Franzen
*=====================================================================
* $Id: FPC.h 3186 2010-09-16 10:40:01Z mike $
*====================================================================*/
#ifndef FPC_H_INCLUDED
#define FPC_H_INCLUDED

#include <stdint.h>
#include <tc1796b/gpta0-struct.h>
#include <tc1796b/gpta1-struct.h>

#define NULL 0

/* FPC operating modes */
enum fpcOpmode_t
{                          /* rising edge         falling edge */
	FPC_DEL_DEB       = 0, /* delayed debounce    delayed debounce */
	FPC_IMM_DEB       = 1, /* immediate debounce  immediate debounce*/
	FPC_IMM_DEB_NO    = 2, /* immediate debounce  no filtering */
	FPC_NO_IMM_DEB    = 3, /* no filtering        immediate debounce */
	FPC_DEL_IMM_DEB   = 4, /* delayed debounce    immediate debounce */
	FPC_IMM_DEL_DEB   = 5, /* immediate debounce  delayed debounce */
	FPC_PRESCALE_RISE = 6, /* prescaler mode */
	FPC_PRESCALE_FALL = 7  /*                     prescaler mode */
};

/* FPC input signal selector */
enum fpcInputSignal_t
{
	FPC_SINk0 = 0, /* signal input SINk0 */
	FPC_SINk1 = 1, /* signal input SINk1 */
	FPC_SINk2 = 2, /* signal input SINk2 */
	FPC_SINk3 = 3, /* signal input SINk3 */
	FPC_SINk4 = 4, /* signal input SINk4 = GPTA module clock */
	FPC_SINk5 = 5  /* signal input SINk5 = preceding FPC output SOLk-1 */
};

/* FPC filter clock selector */
enum fpcFilterClock_t
{
	FPC_CINk0 = 0, /* clock input line 0 = GPTA module clock */
	FPC_CINk1 = 1, /* clock bus line 1 = local PLL clock */
	FPC_CINk2 = 2, /* clock bus line 2 = (prescaled GPTA module clock or PLL
	                  clock from other unit or DCM 3 clock */
	FPC_CINk3 = 3  /* clock bus line 3 = DCM 2 clock or PLL clock from other
	                  unit or uncompensated PLL clock (local or from other
	                  unit) */
};

/* FPC reset timer mode */
enum fpcTimerResetMode_t
{
	FPC_DEC_ON_GLITCH   = 0, /* decrement timer on glitch */
	FPC_CLEAR_ON_GLITCH = 1  /* clear timer on glitch */
};


/* Struct for storing register pointer of the FPC's */
struct fpcRegPointer_t
{
    GPTAn_FPCCTRm_t* ctr; /* control */
    GPTAn_FPCTIMm_t* tim; /* timer */
    GPTAn_FPCSTAT_t* stat; /* status */
};


#define TD_FPC template<uint8_t _GPTA_ID>
#define PL_FPC FPC<_GPTA_ID>

TD_FPC class FPC
{
    public:
        /**
         * \brief Initialize the specified Filter and Prescaler Cell
         * \param fpc_number The FPC instance to use (0-5)
         */
        static void init(uint8_t fpc_number);

        /**
         * \brief Set the FPC operating mode
         * \param fpc_number The FPC instance to use (0-5)
         * \param mode The new operating mode
         */
        static bool setOpmode(uint8_t fpc_number, fpcOpmode_t mode);

        /**
         * \brief Select the FPC input signal
         * \param fpc_number The FPC instance to use (0-5)
         * \param sig The new input signal
         * \return true on success, false else
         */
        static bool selectInputSignal(uint8_t fpc_number, fpcInputSignal_t sig);

        /**
         * \brief Select the FPC filter clock
         * \param fpc_number The FPC instance to use (0-5)
         * \param clk The new filter clock
         * \return true on success, false else
         */
        static bool selectFilterClock(uint8_t fpc_number, fpcFilterClock_t clk);

        /**
         * \brief Set the threshold value of the FPC
         * \param fpc_number The FPC instance to use (0-5)
         * \param threshold The new threshold value
         * \return true on success, false else
         */
        static bool setThreshold(uint8_t fpc_number, uint16_t threshold);

        /**
         * \brief Get the current threshold value of the FPC
         * \param fpc_number The FPC instance to use (0-5)
         * \return The current threshold value
         */
        static uint16_t getThreshold(uint8_t fpc_number);

        /**
         * \brief Set the mode of the timer reset on glitch of the FPC
         * \param fpc_number The FPC instance to use (0-5)
         * \param mode The new mode
         * \return true on success, false else
         * \warning This is effective in Delayed Debounce Filter Mode only.
         */
        static bool setTimerResetMode(uint8_t fpc_number, fpcTimerResetMode_t mode);

        /**
         * \brief Set the timer value of the FPC
         * \param fpc_number The FPC instance to use (0-5)
         * \param value The new value of the timer
         * \return true on success, false else
         */
        static bool setTimerValue(uint8_t fpc_number, uint16_t value);

        /**
         * \brief Get the timer value of the FPC
         * \param fpc_number The FPC instance to use (0-5)
         * \return The current value of the timer
         */
        static uint16_t getTimerValue(uint8_t fpc_number);

        /**
         * \brief Set the value of the Rising Edge Glitch Flag of the FPC
         * \param fpc_number The FPC instance to use (0-5)
         * \param flag Determin if flag will be set (true) or unset (false)
         * \return true on success, false else
         */
        static bool setReg(uint8_t fpc_number, bool flag);

        /**
         * \brief Get the value of the Rising Edge Glitch Flag of the FPC
         * \param fpc_number The FPC instance to use (0-5)
         * \return The current value of the flag
         */
        static bool getReg(uint8_t fpc_number);

        /**
         * \brief Set the value of the Falling Edge Glitch Flag of the FPC
         * \param fpc_number The FPC instance to use (0-5)
         * \param flag Determin if flag will be set (true) or unset (false)
         * \return true on success, false else
         */
        static bool setFeg(uint8_t fpc_number, bool flag);

        /**
         * \brief Get the value of the Falling Edge Glitch Flag of the FPC
         * \param fpc_number The FPC instance to use (0-5)
         * \return The current value of the flag
         */
        static bool getFeg(uint8_t fpc_number);

    private:
        static const fpcRegPointer_t pFpcRegs[6];
};

TD_FPC void PL_FPC::init(uint8_t fpc_number)
{
//	if (fpc_number > 5) osPanic();
//  if ( _GPTA_ID > 1 ) osPanic();

	const fpcRegPointer_t* fpc = &pFpcRegs[fpc_number];

	fpc->ctr->reg = (unsigned long)0;
	fpc->tim->reg = (unsigned long)0;
	fpc->stat->reg = (unsigned long)0;
}


TD_FPC bool PL_FPC::setOpmode(uint8_t fpc_number, fpcOpmode_t mode)
{
    if((pFpcRegs[fpc_number].ctr == NULL) /*|| (mode > 7)*/) return false;

	pFpcRegs[fpc_number].ctr->bits.MOD = mode;
	return true;
}


TD_FPC bool PL_FPC::selectInputSignal(uint8_t fpc_number, fpcInputSignal_t sig)
{
	if((pFpcRegs[fpc_number].ctr == NULL) /*|| (sig > 5)*/) return false;

	pFpcRegs[fpc_number].ctr->bits.IPS = sig;
	return true;
}


TD_FPC bool PL_FPC::selectFilterClock(uint8_t fpc_number, fpcFilterClock_t clk)
{
    if((pFpcRegs[fpc_number].ctr == NULL) /*|| (clk > 3)*/) return false;

	pFpcRegs[fpc_number].ctr->bits.CLK = clk;
	return true;
}


TD_FPC bool PL_FPC::setThreshold(uint8_t fpc_number, uint16_t threshold)
{
	if(pFpcRegs[fpc_number].ctr == NULL) return false;

	pFpcRegs[fpc_number].ctr->bits.CMP = threshold;
	return true;
}


TD_FPC uint16_t PL_FPC::getThreshold(uint8_t fpc_number)
{
	if (pFpcRegs[fpc_number].ctr == NULL)
		return 0; /* there's no reasonable error indicator... */

	return pFpcRegs[fpc_number].ctr->bits.CMP;
}


TD_FPC bool PL_FPC::setTimerResetMode(uint8_t fpc_number, fpcTimerResetMode_t mode)
{
	if((pFpcRegs[fpc_number].ctr == NULL) /*|| (mode > 1)*/) return false;

	pFpcRegs[fpc_number].ctr->bits.RTG = mode;
	return true;
}


TD_FPC bool PL_FPC::setTimerValue(uint8_t fpc_number, uint16_t value)
{
	if (pFpcRegs[fpc_number].tim == NULL) return false;

	pFpcRegs[fpc_number].tim->bits.TIM = value;
	return true;
}


TD_FPC uint16_t PL_FPC::getTimerValue(uint8_t fpc_number)
{
	if (pFpcRegs[fpc_number].tim == NULL)
		return 0; /* again, there's no reasonable error indicator */

	return pFpcRegs[fpc_number].tim->bits.TIM;
}


TD_FPC bool PL_FPC::setReg(uint8_t fpc_number, bool flag)
{
	if (pFpcRegs[fpc_number].stat == NULL) return false;

	if (!flag)
		pFpcRegs[fpc_number].stat->reg &= ~(1 << fpc_number);
	else
		pFpcRegs[fpc_number].stat->reg |= 1 << fpc_number;

	return true;
}


TD_FPC bool PL_FPC::getReg(uint8_t fpc_number)
{
	if (pFpcRegs[fpc_number].stat == NULL)
		return false; /* again, there's no reasonable error indicator */

	if (0 == (pFpcRegs[fpc_number].stat->reg & (1 << fpc_number)))
		return false;
	else
		return true;
}


TD_FPC bool PL_FPC::setFeg(uint8_t fpc_number, bool flag)
{
	if (pFpcRegs[fpc_number].stat == NULL) return false;

	if (!flag)
		pFpcRegs[fpc_number].stat->reg &= ~(1 << (fpc_number + 8));
	else
		pFpcRegs[fpc_number].stat->reg |= 1 << (fpc_number + 8);

	return true;
}


TD_FPC bool PL_FPC::getFeg(uint8_t fpc_number)
{
	if (pFpcRegs[fpc_number].stat == NULL)
		return false; /* again, there's no reasonable error indicator */

	if (0 == (pFpcRegs[fpc_number].stat->reg & (1 << (fpc_number + 8))))
		return false;
	else
		return true;
}

#endif // FPC_H_INCLUDED
