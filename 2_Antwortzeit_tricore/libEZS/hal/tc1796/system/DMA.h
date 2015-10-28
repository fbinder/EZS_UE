/*====================================================================
* Function: DMA abstraction for serial interface ASC0/1
* Description:
*
* Copyright I4Copter Project 2010
* http://www4.informatik.uni-erlangen.de/Research/I4Copter
*=====================================================================
* Author: Martin Oettinger, Martin Hoffmann
*=====================================================================
* $Id: RS232.h 2985 2010-07-16 14:46:02Z sijesche $
*====================================================================*/
#ifndef DMA_H_INCLUDED
#define DMA_H_INCLUDED


#include <machine/wdtcon.h>
#include "../interfaces/RS232.h"

#include <tc1796b/dma.h>
#include <tc1796b/dma-struct.h>
#include <tc1796b/dma/sharemasks.h>

#define TD_DMA template<uint8_t _ASC_ID>
#define PL_DMA DMA<_ASC_ID>

TD_DMA class DMA
{
public:

	static void init();
	static void setup( unsigned int destination_address,  unsigned int memory_access_subrange_mask, unsigned char buffer_length);
	static void reset();
	static void reset(unsigned char channel);

	static void config_pattern_irq( unsigned char pattern1, unsigned char pattern2, unsigned char srpn);
	static void config_channel_irq(unsigned char srpn);
	static void enable_irq();
	static void disable_irq();
	static void asc_start();
    static void set_transfers_per_transaction( unsigned int num);
	static void set_destination_buffer(unsigned int address);
	static int  get_received_bytes();

private:
	static DMA_t* const dma_base;
};


// Init pointer...
TD_DMA DMA_t* const PL_DMA::dma_base = (DMA_t*) DMA_BASE;


TD_DMA
void PL_DMA::init(){
}

/**
 * DMA reset, see TriCore manual p.12-16
 */
TD_DMA
void PL_DMA::reset(){
		switch (_ASC_ID) {
				case 0:
						dma_base->HTREQ.bits.ECH04=0;
						break;
				case 1:
						dma_base->HTREQ.bits.ECH05=0;
						break;
		}

		do{
				dma_base->CHRSTR.reg |= (1<<(_ASC_ID+4));
		}while(dma_base->CHRSTR.reg & (1<<(_ASC_ID+4)));
}
#if 0

TD_DMA
void PL_DMA::reset(unsigned char channel){
		dma_base->HTREQ.reg &= ~(1<<channel);

		do{
				dma_base->CHRSTR.reg |= (1<<channel);
		}while(dma_base->CHRSTR.reg & (1<<channel));
}
#endif

TD_DMA
void PL_DMA::enable_irq(){
  switch (_ASC_ID) {
    case 0:
      dma_base->SRC0.bits.SRE = 1;
      break;
    case 1:
      dma_base->SRC1.bits.SRE = 1;
      break;
    case 2:		// SensorAddon 
      dma_base->SRC2.bits.SRE = 1; 
      break;
  }
}

TD_DMA
void PL_DMA::disable_irq(){
  switch(_ASC_ID) {
    case 0:
      dma_base->SRC0.bits.SRE = 0;
      break;
    case 1:
      dma_base->SRC1.bits.SRE = 0;
      break;
    case 2:		// SensorAddon 
      dma_base->SRC2.bits.SRE = 0; 
      break;  }
}

#if 0
TD_DMA
void PL_DMA::enable_irq(unsigned char channel){
	DMA_SRCm_t* SRCx = (&(dma_base->SRC0)) = channel;
	SRCx.bits.SRE=0;
}

TD_DMA
void PL_DMA::disable_irq(unsigned char channel){
	DMA_SRCm_t* SRCx = (&(dma_base->SRC0)) = channel;
	SRCx.bits.SRE=0;
}
#endif
TD_DMA
void PL_DMA::asc_start() {
  /* only support for hardware-mode */
  switch (_ASC_ID) {
    case 0:
      dma_base->SRC0.bits.CLRR = 1;
      dma_base->HTREQ.bits.ECH04=1;
      break;
    case 1:
      dma_base->SRC1.bits.CLRR = 1;
      dma_base->HTREQ.bits.ECH05=1;
      break;
  }
}

#if 0
TD_DMA
void PL_DMA::ssc_start() {
  /* only support for hardware-mode */
      dma_base->SRC2.bits.CLRR = 1;
      dma_base->HTREQ.bits.ECH15=1;
}
#endif


TD_DMA
void PL_DMA::config_channel_irq(unsigned char srpn){

  switch (_ASC_ID) {
    case 0:
	/* Channel Interrupt  an Service Request Output-Knoten (0-7) leiten */
	dma_base->CH[4].CHICR.bits.INTP=0;

	/* Interupt Raise Detect Value*/
	dma_base->CH[4].CHICR.bits.IRDV = 0;
	/* Interrupt when TCOUNT == IRDV */
	dma_base->CH[4].CHICR.bits.INTCT = 2;

	/* Konfiguration des Service Request Output Knotens */
	dma_base->SRC0.bits.SRPN = srpn;
      	dma_base->SRC0.bits.TOS = 0;
      	dma_base->SRC0.bits.SRE = 0;
      	break;
    case 1:
	/* Channel Interrupt  an Service Request Output-Knoten (0-7) leiten */
	dma_base->CH[5].CHICR.bits.INTP=1;

	/* Interupt Raise Detect Value*/
	dma_base->CH[5].CHICR.bits.IRDV = 0;
	/* Interrupt when TCOUNT == IRDV */
	dma_base->CH[5].CHICR.bits.INTCT = 2;

	/* Konfiguration des Service Request Output Knotens */
	dma_base->SRC1.bits.SRPN = srpn;
      	dma_base->SRC1.bits.TOS = 0;
      	dma_base->SRC1.bits.SRE = 0;
      	break;
     }

}

TD_DMA
void PL_DMA::config_pattern_irq( unsigned char pattern1, unsigned char pattern2, unsigned char srpn)
{

  switch (_ASC_ID) {
    case 0:
/* pattern compare actual  RDm0 to PATm0, masked by PATm2  -- previous RDm0 to PATm1, masked by PATm3*/
      dma_base->CH[4].CHCR.bits.PATSEL=3;
      dma_base->ME0PR.bits.PAT00 = pattern2; // lf
      dma_base->ME0PR.bits.PAT02 = 0;
      dma_base->ME0PR.bits.PAT01 = pattern1; // cr
      dma_base->ME0PR.bits.PAT03 = 0;

/* Wrap/Pattern-Interrupt an Service Request Output-Knoten (0-7) leiten */
      dma_base->CH[4].CHICR.bits.WRPP=0;
      dma_base->CH[4].CHICR.bits.INTP=0;

/* Konfiguration des Service Request Output Knotens */
      dma_base->SRC0.bits.SRPN = srpn;
      dma_base->SRC0.bits.TOS = 0;
      dma_base->SRC0.bits.SRE = 0;
      break;
    case 1:
/* pattern compare actual  RDm0 to PATm0, masked by PATm2  -- previous RDm0 to PATm1, masked by PATm3*/
      dma_base->CH[5].CHCR.bits.PATSEL=2;
      dma_base->ME0PR.bits.PAT00 = pattern2; // lf
      dma_base->ME0PR.bits.PAT02 = 0;
      dma_base->ME0PR.bits.PAT01 = pattern1; // cr
      dma_base->ME0PR.bits.PAT03 = 0;

/* Wrap/Pattern-Interrupt an Service Request Output-Knoten (0-7) leiten */
      dma_base->CH[5].CHICR.bits.WRPP=1;
      dma_base->CH[5].CHICR.bits.INTP=1;

/* Konfiguration des Service Request Output Knotens */
      dma_base->SRC1.bits.SRPN = srpn;
      dma_base->SRC1.bits.TOS = 0;
      dma_base->SRC1.bits.SRE = 0;
      break;
  }

}

TD_DMA
void PL_DMA::setup(unsigned int destination_address,  unsigned int memory_access_subrange_mask, unsigned char buffer_length){

  unsigned char channel = 0;

  switch(_ASC_ID) {
    case 0:
      channel=4;
      unlock_wdtcon();
      dma_base->ME0AENR.reg |= (  DMA_MEmAENR_AEN18_MASK | DMA_MEmAENR_AEN5_MASK );
      dma_base->ME0ARR.reg=memory_access_subrange_mask;
      lock_wdtcon();
	/* set the base source address for DMA moves */
        dma_base->CH[4].SADR.reg=RS232<ASC0_ID>::get_rbuf();

	/* Hardware controlled continuous mode for channel  */
	  dma_base->CH[channel].CHCR.bits.CHMODE = 1; // 0 single
	/* number of dma transfers for a transaction */
	  dma_base->CH[channel].CHCR.bits.TREL = 256;
    break;
    case 1:
      channel=5;
      unlock_wdtcon();
      dma_base->ME0AENR.reg |= ( DMA_MEmAENR_AEN18_MASK | DMA_MEmAENR_AEN6_MASK );
      dma_base->ME0ARR.reg=memory_access_subrange_mask;
      lock_wdtcon();
	/* Hardware controlled single mode for channel  */
	dma_base->CH[channel].CHCR.bits.CHMODE = 0;
	/* set the base source address for DMA moves */
	dma_base->CH[5].SADR.reg=RS232<ASC1_ID>::get_rbuf();

	/* number of dma transfers for a transaction */
	  dma_base->CH[channel].CHCR.bits.TREL = 16;
      break;
  }


/* Each DMA transfer has to be requested by a hardware request signal */
  dma_base->CH[channel].CHCR.bits.RROAT = 0;

/* Selecting asc_rir as hardware request input */
  dma_base->CH[channel].CHCR.bits.PRSEL = 0x03;

/* number of dma moves executed during one transfer (0 means 1) */
  dma_base->CH[channel].CHCR.bits.BLKM = 0;

/* set the base destination address for DMA moves */
  dma_base->CH[channel].DADR.reg=destination_address;

/* set data width to 8 bit */
  dma_base->CH[channel].CHCR.bits.CHDW=0;

/* set destination address modification factor to 1xCHDW */
  dma_base->CH[channel].ADRCR.bits.DMF=0;

/* add offset of DMF after each DMA move */
  dma_base->CH[channel].ADRCR.bits.INCD=1;

/* length of the circular source address buffer; 0 uses always the same source
   adress */
  dma_base->CH[channel].ADRCR.bits.CBLS=0;

/* length of the circular destination address buffer (bits not to modify) (32 bit words -> 4 Words == 16 chars) */
  dma_base->CH[channel].ADRCR.bits.CBLD=buffer_length;

}

TD_DMA
int PL_DMA::get_received_bytes(){
	 return	dma_base->CH[_ASC_ID + 4].CHCR.bits.TREL - dma_base->CH[_ASC_ID + 4].CHSR.bits.TCOUNT;
}

TD_DMA
void PL_DMA::set_transfers_per_transaction(unsigned int num){
	dma_base->CH[_ASC_ID+4].CHCR.bits.TREL = num;
}

TD_DMA
void PL_DMA::set_destination_buffer(unsigned int address){
	dma_base->CH[_ASC_ID+4].DADR.reg=address;
}

#endif

