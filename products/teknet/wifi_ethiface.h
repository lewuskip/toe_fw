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

typedef struct wifi_ethface_t {
	uint8_t autoconf;
}wifi_ethface_t;

#ifndef PRODUCTS_TEKNET_WIFI_ETHIFACE_H_
#define PRODUCTS_TEKNET_WIFI_ETHIFACE_H_



int wifi_ethiface(uint8_t autoconf);


#endif /* PRODUCTS_TEKNET_WIFI_ETHIFACE_H_ */
