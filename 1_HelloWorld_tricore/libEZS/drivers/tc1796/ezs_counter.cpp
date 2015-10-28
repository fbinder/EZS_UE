#include "ezs_counter.h"
#include "SystemTimer.h"
#include "math.h"
extern "C" {

void ezs_counter_init(void) {
	SystemTimer::init();
}

cyg_uint64 ezs_counter_get(void) {
	return SystemTimer::timer0();
}

cyg_uint64 ezs_counter_resolution_us(void){
	return (cyg_uint64) ceil( (float) ezs_counter_resolution_ps() /1000000.0);
}

cyg_uint64 ezs_counter_resolution_ps(void)
{
	/**
	 * If Fsys is 75MHz one tick lasts 13.3 ns
	 * This is hardcoded for the moment
	 **/
	return 13300;
}


}
