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

typedef enum ethface_state_t {
	ETH_IFACE_DISCONNECTED = 0,
	ETH_IFACE_CONNECTED
}ethface_state_t;


typedef void (*ethiface_cb_t)(void *);


typedef struct wifi_ethface_t {
	uint8_t autoconf;
	struct station_config stationConf;
	ethface_state_t state;
	ethiface_cb_t clb;
}wifi_ethface_t;

#ifndef PRODUCTS_TEKNET_WIFI_ETHIFACE_H_
#define PRODUCTS_TEKNET_WIFI_ETHIFACE_H_


uint8_t wifi_ethiface_is_connected(void);
int wifi_ethiface_enable(void);
int wifi_ethiface(uint8_t autoconf);


#endif /* PRODUCTS_TEKNET_WIFI_ETHIFACE_H_ */
