#include <cyg/hal/hal_arch.h>
#include <cyg/kernel/kapi.h>
#include <cyg/infra/diag.h>

#include <stdio.h>
#include <iso646.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "ezs_counter.h"
#include "ezs_gpio.h"
#include "ezs_stopwatch.h"
#include "ezs_rs232.h"
#include "ezs_dac.h"
#include "ezs_io.h"

#define STACKSIZE CYGNUM_HAL_STACK_SIZE_MINIMUM+102400

#define RS232_IRQ 1
static cyg_interrupt rs232_intr;
static cyg_handle_t rs232_isr_handle;

static cyg_uint32 rs232_isr_handler(cyg_vector_t vector, cyg_addrword_t data)
{
  /* TODO: FIX ISR!!!! */
	uint8_t input[2] = { 0, 0 };
	input[0] = ezs_rs232_getc();
	ezs_rs232_acknowledge_rx_interrupt();
	diag_printf((char *) input);
	return CYG_ISR_HANDLED | CYG_ISR_CALL_DSR;
}

static void rs232_dsr_handler(cyg_vector_t vector, cyg_ucount32 count,
			      cyg_addrword_t data)
{
  /* Implement */
}

static void rs232_thread(cyg_addrword_t arg)
{
  /* Implement */
}

static void signal_generator_thread(cyg_addrword_t arg)
{
  /* Implement */
}

void cyg_user_start(void)
{
	// Setup counter
	ezs_counter_init();
	ezs_dac_init();
	ezs_rs232_init(115200, RS232_IRQ);

  /* Create signal_generator_thread */
  /* Implement */
  
  /* Create rs232_thread */
  /* Implement */

	// Create ISR + DSR
	cyg_interrupt_create(RS232_IRQ,
						 1,
						 0,
						 rs232_isr_handler,
						 rs232_dsr_handler,
						 &rs232_isr_handle,
						 &rs232_intr) ;
	cyg_interrupt_attach(rs232_isr_handle);
	cyg_interrupt_unmask(RS232_IRQ);

}
