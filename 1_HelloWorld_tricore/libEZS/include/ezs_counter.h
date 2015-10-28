#ifndef EZS_COUNTER_H_INCLUDED
#define EZS_COUNTER_H_INCLUDED

#include <cyg/infra/cyg_type.h>

#ifdef __cplusplus
extern "C" {
#endif
/*!
 * \brief This function has to be called exactly once so ezs_counter_get()
 *        yields meaningfull results.
 */
void ezs_counter_init(void);

/**
 * \brief Read the current counter value from the hardware counter.
 * @return counter value (for time resolution \see ezs_counter_resolution_us)
 */
cyg_uint64 ezs_counter_get(void);

/**
 * \brief Returns the current time resolution of the hardware counter.
 * @return tick resolution in microseconds
 */
cyg_uint64 ezs_counter_resolution_us(void);
/**
 * \brief Returns the current time resolution of the hardware counter.
 * @return tick resolution in picoseconds
 */
cyg_uint64 ezs_counter_resolution_ps(void);

#ifdef __cplusplus
}
#endif

#endif // EZS_COUNTER_H_INCLUDED
