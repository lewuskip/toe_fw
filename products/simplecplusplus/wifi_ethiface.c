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
		.autoconf = 0,
		.state = ETH_IFACE_DISCONNECTED,
		.clb = NULL
};

static const int LED = 2;
static volatile os_timer_t some_timer_wifi;

const char ssid[32] = "smacznego";
const char password[32] = "dziekuje";

void ICACHE_FLASH_ATTR some_timerfunc(void	*arg){
	if (GPIO_INPUT_GET(LED) == 0) {
		GPIO_OUTPUT_SET(LED, 1);
	}
	else {
		GPIO_OUTPUT_SET(LED, 0);
	}
}


void wifi_cb(System_Event_t *evt)
{
	ethface_state_t curr_state = wifi.state;

	switch(wifi_station_get_connect_status())
	{
		case STATION_IDLE:
			wifi.state = ETH_IFACE_DISCONNECTED;
			break;

		case STATION_CONNECTING:
			os_printf("STATION_CONNECTING\n");
			wifi.state = ETH_IFACE_DISCONNECTED;
			break;

		case STATION_WRONG_PASSWORD:
			os_printf("STATION_WRONG_PASSWORD\n");
			wifi.state = ETH_IFACE_DISCONNECTED;
			break;

		case STATION_NO_AP_FOUND:
			os_printf("STATION_NO_AP_FOUND\n");
			wifi.state = ETH_IFACE_DISCONNECTED;
			break;

		case STATION_CONNECT_FAIL:
			os_printf("STATION_CONNECT_FAIL\n");
			wifi.state = ETH_IFACE_DISCONNECTED;
			break;

		case STATION_GOT_IP:
			wifi.state = ETH_IFACE_CONNECTED;
			os_printf("STATION_GOT_IP\n");
			break;

		default:
			os_printf("Bad!\n");
			break;

	}

	if ((wifi.state != curr_state) && (wifi.clb != NULL)) {
		wifi.clb(NULL);
	}
}

uint8_t wifi_ethiface_is_connected(void)
{
	if (wifi.state == ETH_IFACE_CONNECTED)
	{
		return 1;
	}
	else {
		return 0;
	}
}

int registerClb(ethiface_cb_t clb)
{
	wifi.clb = clb;
	return 0;
}

int wifi_ethiface_enable(void)
{
	wifi.stationConf.bssid_set = 0;
	os_memcpy(&wifi.stationConf.ssid,	ssid,	32);
	os_memcpy(&wifi.stationConf.password,	password,	32);
	wifi_set_opmode(1);
	wifi_set_event_handler_cb(&wifi_cb);
	wifi_station_set_config_current(&wifi.stationConf);
	return 0;
}

int wifi_ethiface(uint8_t autoconf)
{
	wifi.autoconf = autoconf;

	// Disaconnect
	wifi_station_disconnect();
	wifi_set_opmode(NULL_MODE);

	os_timer_setfn((os_timer_t *)&some_timer_wifi, (os_timer_func_t *)some_timerfunc, NULL);
	os_timer_arm((os_timer_t *)&some_timer_wifi, 100	, 1);

	return 0;
}




