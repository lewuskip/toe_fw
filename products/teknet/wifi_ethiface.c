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

#include "wifi_ethiface.h"

wifi_ethface_t wifi = {
		.autoconf = 0
};


int wifi_ethiface(uint8_t autoconf)
{
	wifi.autoconf = autoconf;

	return 0;
}




