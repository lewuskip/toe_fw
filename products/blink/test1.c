#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_interface.h"
#include "driver/uart.h"
#include "espconn.h"
#include "mem.h"

void ICACHE_FLASH_ATTR
print_debug(void)
{
	os_printf("DEBUG\n");
}
