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

void test_thread(cyg_addrword_t arg){
	diag_printf("Hallo Welt!\n");
	/* Einmalige Aufgaben ... */

	while(1){
	/* Periodische Aufgaben ... */

	}

}

static void hello_world(cyg_addrword_t data){
	int message = (int) data;
	printf("Beginning hello world excecution: thread data is %d\n", message);
	for(;;){
		diag_printf("Hello World!\n");
		cyg_thread_delay(1000);
	}
}

void cyg_user_start(void){
	ezs_dac_init();

	/* Thread erzeugen ... */
	diag_printf("Entering cyg_user_start() function\n");
	cyg_thread_create(PRIORITY, &hello_world, 0, "thread 1", my_stack, STACKSIZE, &handle, &threaddata);

	/* Thread starten ... */
	cyg_thread_resume(&handle);
}

