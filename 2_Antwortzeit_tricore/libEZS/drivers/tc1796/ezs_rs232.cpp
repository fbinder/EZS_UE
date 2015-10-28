#include "ezs_rs232.h"
#include "RS232DeviceImpl.h"
#include <tc1796b/asc0.h>

extern "C" {

hal::tc1796::RS232DeviceImpl device = hal::tc1796::RS232DeviceImpl(0);

void ezs_rs232_init(uint32_t baudrate, uint8_t irq) {
#if 0
	device.init(baudrate);
#endif
	/*
	 * set interrupt priority
	 */
	ASC0_RSRC.bits.SRPN = irq;
	/*
	 * enable interrupt
	 */
	ASC0_RSRC.bits.SRE = 1;

}

void ezs_rs232_putc(uint8_t c) {
	device.send(c);
}

uint8_t ezs_rs232_getc(void) {
	return ASC0_RBUF.bits.RD_VALUE;
}

void ezs_rs232_acknowledge_rx_interrupt(void) {
	ASC0_RSRC.bits.CLRR = 1;
}

}
