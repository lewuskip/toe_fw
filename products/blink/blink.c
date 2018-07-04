#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_interface.h"
#include "driver/uart.h"
#include "espconn.h"
#include "mem.h"

#include "test1.h"

static volatile os_timer_t some_timer;

uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        case FLASH_SIZE_64M_MAP_1024_1024:
            rf_cal_sec = 2048 - 5;
            break;
        case FLASH_SIZE_128M_MAP_1024_1024:
            rf_cal_sec = 4096 - 5;
            break;
        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

static const int LED = 2;

void ICACHE_FLASH_ATTR some_timerfunc(void	*arg){
	if (GPIO_INPUT_GET(LED) == 0) {
		GPIO_OUTPUT_SET(LED, 1);
	}
	else {
		GPIO_OUTPUT_SET(LED, 1);
	}
}

void ICACHE_FLASH_ATTR
user_init(void)
{
	gpio_init();
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U,FUNC_GPIO2);
	gpio_output_set(0, (1 << LED), (1 << LED), 0);
	os_timer_setfn((os_timer_t *)&some_timer, (os_timer_func_t *)some_timerfunc, NULL);
	os_timer_arm((os_timer_t *)&some_timer, 500	, 1);
}
