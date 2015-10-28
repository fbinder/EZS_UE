#ifndef EZS_IO_H_INCLUDED
#define EZS_IO_H_INCLUDED

#include <cyg/infra/cyg_type.h>

/**
 * \file ezs_io.h
 * \brief Input/Output primitives
 */

/**
 * \brief print value annotated with timestamp. The exact means of printing are
 * subject to the underlying hardware.
 */
void ezs_print_measurement(cyg_uint32 timestamp, cyg_uint32 value);

#endif // EZS_IO_H_INCLUDED