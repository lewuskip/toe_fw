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
#include <mem.h>

#include "wifi_ethiface.h"

#define MAX_NUM_OF_NODES 4

typedef enum node_status_t {
	NODE_DISCONNECTED = 0,
	NODE_CONNECTING,
	NODE_CONNECTED
}node_status_t;

typedef struct node_t {
	struct espconn 	espconn;
	bool			active;
	esp_tcp 		esptcp;
	node_status_t	status;
}node_t;


typedef struct eventNodeManager_t {
	struct espconn 	espconn;
	esp_tcp 		esptcp;
	os_timer_t 		timer;
	node_t			nodes[MAX_NUM_OF_NODES];
	uint8_t			num_of_nodes;
}eventNodeManager_t;

static eventNodeManager_t manager;

static node_t *getNode(struct espconn *ptr)
{
	uint8_t idx;
	node_t *node;

	for (idx=0 ; idx<manager.num_of_nodes ; idx++)
	{
		node = &manager.nodes[idx];
		if (node->active)
		{
			if (node->esptcp.remote_ip[0] == ptr->proto.tcp->remote_ip[0] &&
				node->esptcp.remote_ip[1] == ptr->proto.tcp->remote_ip[1] &&
				node->esptcp.remote_ip[2] == ptr->proto.tcp->remote_ip[2] &&
				node->esptcp.remote_ip[3] == ptr->proto.tcp->remote_ip[3] &&
				node->esptcp.remote_port == ptr->proto.tcp->remote_port )
			{
				return node;
			}
		}
	}
	return NULL;
}

LOCAL void ICACHE_FLASH_ATTR
user_tcp_connect_cb(void *arg)
{
    struct espconn *pespconn = arg;
	node_t *node;

	node = (node_t *)os_malloc(1);
	node = getNode(pespconn);
	if (node == NULL) {
		os_printf("Connect to unknown node\n");
	}
	else
	{
		node->status = NODE_CONNECTED;
	}

    //espconn_send(&manager.esp_conn, "BLE", 3);
}

LOCAL void ICACHE_FLASH_ATTR
user_tcp_discon_cb(void *arg)
{
    struct espconn *pespconn = arg;
	node_t *node;

	node = getNode(pespconn);
	if (node == NULL) {
		os_printf("Disconnect to unknown node\n");
	}
	else
	{
		node->status = NODE_DISCONNECTED;
	}
}

LOCAL void ICACHE_FLASH_ATTR
user_tcp_recon_cb(void *arg)
{
    struct espconn *pespconn = arg;
    os_printf("Error\n");
}


int new_node(uint8_t *ipaddr, uint32_t remote_port)
{
	node_t *new_node;

	os_printf("remote_port\n");
	if (manager.num_of_nodes >= MAX_NUM_OF_NODES ) {
		os_printf("Exceeeded max number of nodes\n");
		return -1;
	}

	new_node = &manager.nodes[manager.num_of_nodes++];

	os_printf("Creating new node\n");

	if (NULL == new_node) return -1;

	new_node->status = NODE_DISCONNECTED;
	new_node->esptcp.local_port = 1000 + manager.num_of_nodes;
	new_node->esptcp.remote_ip[0] = ipaddr[0];
	new_node->esptcp.remote_ip[1] = ipaddr[1];
	new_node->esptcp.remote_ip[2] = ipaddr[2];
	new_node->esptcp.remote_ip[3] = ipaddr[3];
	new_node->esptcp.remote_port = remote_port;

	new_node->espconn.type = ESPCONN_TCP;
	new_node->espconn.proto.tcp = &manager.esptcp;
	new_node->espconn.state = ESPCONN_NONE;
	new_node->active = true;
	espconn_regist_connectcb(&new_node->espconn, user_tcp_connect_cb); // register connect callback
	espconn_regist_disconcb(&new_node->espconn, user_tcp_discon_cb);

	if (wifi_ethiface_is_connected() == true)
	{
		if(espconn_connect(&new_node->espconn) != 0)
		{
			os_printf("Can't connect new node\n");
			return -1;
		}
	}

	return 0;
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

	manager.espconn.type = ESPCONN_TCP;
	manager.espconn.proto.tcp = &manager.esptcp;
	manager.espconn.state = ESPCONN_NONE;

	espconn_regist_connectcb(&manager.espconn, user_tcp_connect_cb); // register connect callback
	espconn_regist_disconcb(&manager.espconn, user_tcp_discon_cb);
	//espconn_regist_reconcb(&manager.esp_conn, user_tcp_recon_cb); // register reconnect callback as error handler

	if(espconn_connect(&manager.espconn) == 0) // Correctly setup
	{
			os_printf("Connected\n");
		return 0;
	}
	else {
		os_printf("Not connected\n");

	}
	return 1;
}

void evaluateNodes(void)
{
	uint8_t idx;
	node_t *node;

	for (idx=0 ; idx<manager.num_of_nodes ; idx++)
	{
		node = &manager.nodes[idx];

		os_printf("Node %d\n", idx);
		if (node->status == NODE_DISCONNECTED) {
			if(espconn_connect(&node->espconn) != 0)
			{
				os_printf("Connecting node failed - critical\n");
			}
			else {
				node->status = NODE_CONNECTING;
				os_printf("Node(%d) connecting to %d.%d.%d.%d:%d\n", idx,
						node->esptcp.remote_ip[0],
						node->esptcp.remote_ip[1],
						node->esptcp.remote_ip[2],
						node->esptcp.remote_ip[3],
						node->esptcp.remote_port);
			}
		}
	}
}

void ICACHE_FLASH_ATTR nodeTimer(void	*arg)
{
	if (wifi_ethiface_is_connected() == true) {
			evaluateNodes();
		}
}

void ethIfaceCallBack(void *attr)
{
	if (wifi_ethiface_is_connected() == true) {
		evaluateNodes();
	}
}

int eventNodeInit(void)
{
	uint8_t ip[4] = {192,168,1,110};

	os_timer_setfn((os_timer_t *)&manager.timer, (os_timer_func_t *)nodeTimer, NULL);
	os_timer_arm((os_timer_t *)&manager.timer, 1000	, 1);

	new_node(ip, 8003);
	registerClb(&ethIfaceCallBack);
	return 1;
}
