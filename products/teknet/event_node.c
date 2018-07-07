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
#include "osapi.h"
#include "user_interface.h"
#include "espconn.h"

#include "wifi_ethiface.h"

typedef struct eventNodeManager_t {
	struct espconn 	esp_conn;
	esp_tcp 		esptcp;
	os_timer_t 		timer;
}eventNodeManager_t;

static eventNodeManager_t manager;

LOCAL void ICACHE_FLASH_ATTR
user_tcp_connect_cb(void *arg)
{
    struct espconn *pespconn = arg;

    os_printf("connect succeed!\n");
    //espconn_send(&eventConn, "BLE", 3);
}

LOCAL void ICACHE_FLASH_ATTR
user_tcp_discon_cb(void *arg)
{
    struct espconn *pespconn = arg;
    os_printf("Disconnected\n");
}

LOCAL void ICACHE_FLASH_ATTR
user_tcp_recon_cb(void *arg)
{
    struct espconn *pespconn = arg;
    os_printf("Error\n");
}

int evnetNodeReConnect(void)
{
	os_printf("evnetNodeReConnect");
	manager.esptcp.local_port = 1010;
	manager.esptcp.remote_ip[0] = 192;
	manager.esptcp.remote_ip[1] = 168;
	manager.esptcp.remote_ip[2] = 1;
	manager.esptcp.remote_ip[3] = 110;
	manager.esptcp.remote_port = 8003;

	manager.esp_conn.type = ESPCONN_TCP;
	manager.esp_conn.proto.tcp = &manager.esptcp;
	manager.esp_conn.state = ESPCONN_NONE;

	espconn_regist_connectcb(&manager.esp_conn, user_tcp_connect_cb); // register connect callback
	espconn_regist_disconcb(&manager.esp_conn, user_tcp_discon_cb);
	//espconn_regist_reconcb(&manager.esp_conn, user_tcp_recon_cb); // register reconnect callback as error handler

	if(espconn_connect(&manager.esp_conn) == 0) // Correctly setup
	{
			os_printf("Connected\n");
		return 0;
	}
	else {
		os_printf("Not connected\n");

	}
	return 1;
}

void ICACHE_FLASH_ATTR nodeTimer(void	*arg){

}

void ethIfaceCallBack(void *attr)
{
	os_orintf("Etg new status\n");

}

int eventNodeInit(void)
{
	//evnetNodeReConnect();

	os_timer_setfn((os_timer_t *)&manager.timer, (os_timer_func_t *)nodeTimer, NULL);
	os_timer_arm((os_timer_t *)&manager.timer, 1000	, 1);

	registerClb(&ethIfaceCallBack);
	return 1;
}
