#include "ezs_io.h"

#include <cyg/infra/diag.h>

#include <string.h>
#include <stdio.h>

void ezs_print_measurement(cyg_uint32 timestamp, cyg_uint32 value) {
    const size_t buffer_length = 10   /* first number */
                                 + 1  /* separator */
                                 + 10 /* second number */
                                 + 1  /* newline  */
                                 + 1  /* terminating null byte  */ ;
    char buffer[buffer_length];
    memset(buffer, 0, buffer_length);
    snprintf(buffer, buffer_length, "%u;%u\n", (unsigned int)timestamp,
             (unsigned int)value);
    diag_printf(buffer);
}

