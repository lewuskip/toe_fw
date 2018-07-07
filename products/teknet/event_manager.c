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

#include "event_manager.h"

typedef struct eventManager_t {
	uint8_t numOfNodes;
	uint32_t currIdx;
}eventManager_t;

typedef struct eventMessage_t {
	void *pyaload;
}eventMessage_t;


static eventManager_t manager = {
		.numOfNodes = 0,
		.currIdx = 0,
};

int newEvent(eventType_t event)
{
	os_printf("new event\n");

}

int  eventManagerInit(void)
{

	return 0;
}
