/*
 * Copyright:
 * ----------------------------------------------------------------------------
 * This confidential and proprietary software may be used only as authorized
 * by a licensing agreement from ThingsOnEdge Limited.
 *      (C) COPYRIGHT 2018 ThingsOnEdge Limited, ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorized copies and
 * copies may only be made to the extent permitted by a licensing agreement
 * from ThingsOnEdge Limited.
 * ----------------------------------------------------------------------------
*/

#include "os_type.h"
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "user_interface.h"

#include "wifi_ethiface.h"

wifi_ethface_t wifi = {
		.autoconf = 0
};

static const int LED = 2;
static volatile os_timer_t some_timer;

void ICACHE_FLASH_ATTR some_timerfunc(void	*arg){
	if (GPIO_INPUT_GET(LED) == 0) {
		GPIO_OUTPUT_SET(LED, 1);
	}
	else {
		GPIO_OUTPUT_SET(LED, 0);
	}
}

int wifi_ethiface(uint8_t autoconf)
{
	wifi.autoconf = autoconf;
	os_timer_setfn((os_timer_t *)&some_timer, (os_timer_func_t *)some_timerfunc, NULL);
	os_timer_arm((os_timer_t *)&some_timer, 500	, 1);
	return 0;
}




