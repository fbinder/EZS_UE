#ifndef EZS_RS232_H_INCLUDED
#define EZS_RS232_H_INCLUDED

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file ezs_rs232.h
 * \ingroup hw
 * \brief Functions for using the RS232 interface.
 */

/**
 * \brief initializes the RS232 interface. Call this function exactly once
 * before using any other function provided by this module.
 */
void ezs_rs232_init(uint32_t /* baudrate */, uint8_t /* irq */);

/**
 * \brief writes a single byte to the RS232 interface.
 */
void ezs_rs232_putc(uint8_t /* c */);

/**
 * \brief consumes a single byte from the RS232 interface's input buffer.
 */
uint8_t ezs_rs232_getc(void);

/**
 * \brief Marks the current RS232 interrupt as handled.
 */
void ezs_rs232_acknowledge_rx_interrupt(void);

#ifdef __cplusplus
}
#endif

#endif // EZS_RS232_H_INCLUDED
