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

#ifndef PRODUCTS_TEKNET_EVENT_MANAGER_H_
#define PRODUCTS_TEKNET_EVENT_MANAGER_H_

typedef enum eventType_t {
	EVENT_BUTTON_PRESSED = 0
}eventType_t;

int newEvent(eventType_t event);


#endif /* PRODUCTS_TEKNET_EVENT_MANAGER_H_ */
