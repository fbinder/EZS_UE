#include <cyg/hal/hal_arch.h>
#include <cyg/kernel/kapi.h>
#include <cyg/infra/diag.h>

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <iso646.h>

#include "ezs_dac.h"
#include "ezs_gpio.h"


/* Thread-Stack */
#define PRIORITY 11
#define STACKSIZE CYGNUM_HAL_STACK_SIZE_MINIMUM+1024

static cyg_uint8 my_stack[STACKSIZE];
static cyg_handle_t handle;
static cyg_thread threaddata;

//Funktion eventuell static machen
void test_thread(cyg_addrword_t arg){
	/* Einmalige Aufgaben ... */
	int message = (int) arg;
	printf("Beginning hello world excecution: thread data is %d\n", message);

	while(1){
		/* Periodische Aufgaben ... */
		diag_printf("Hello World!\n");
		cyg_thread_delay(1000);
	}
}

void cyg_user_start(void){
	ezs_dac_init();

	/* Thread erzeugen ... */
	diag_printf("Entering cyg_user_start() function\n");
	cyg_thread_create(PRIORITY, &test_thread, 0, "thread 1", my_stack, STACKSIZE, &handle, &threaddata);

	/* Thread starten ... */
	cyg_thread_resume(&handle);
}


/*#include<stdio.h>
#include<math.h>

//gcc <file> -lm
//-lm in order to load math.h seperately!!

int main() {
	float i = 0.0;
	int steps = 360;
	
	for (i=0.0; i<=(2*M_PI); i=i+(2*M_PI/steps)){
		//sinf() results in [-1,1]!
		printf("Sin(%f): %f°\n", i*180/M_PI , sinf(i));
	}
	
	return 0;
}*/
