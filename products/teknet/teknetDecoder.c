/*
 * teknetDecoder.c
 *
 *  Created on: 28 Jun 2018
 *      Author: lewy
 */

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_interface.h"

void newEvent(uint32_t eventIdx);

void ICACHE_FLASH_ATTR hw_timer_init(int source_type, u8 req);


typedef struct teknetDcr{
	uint8_t inPin;
	uint8_t bitIdx;
	uint32_t value;
	uint32_t idx;
}teknetDcr_t;

static teknetDcr_t device;
static const int LED = 4;
void irqHandler(void *args);

void debugLine(void)
{
	gpio_output_set((1 << LED), 0, 0, 0);
	os_delay_us(2);
	gpio_output_set(0, (1 << LED), 0, 0);
}

void hwTimer(void)
{
	uint32	gpio_status;

	device.value |= (GPIO_INPUT_GET(device.inPin)<<device.bitIdx);

	device.bitIdx++;
	if ( device.bitIdx >= 32) {
		gpio_status	= GPIO_REG_READ(GPIO_STATUS_ADDRESS);
		//clear	interrupt	status
		GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);
		ETS_GPIO_INTR_ENABLE();
		ETS_GPIO_INTR_ATTACH(irqHandler, NULL);
		hw_timer_set_func(NULL);
		if ( (device.value &0xFF000000) == 0x44000000) {
			os_printf("Ring %d\n", device.idx++);
			newEvent(device.idx);
		}
	}
	else {

		hw_timer_arm(490);
	}
}


void irqHandler(void *args)
{
	uint32	gpio_status;

	ETS_GPIO_INTR_DISABLE();
	ETS_GPIO_INTR_ATTACH(NULL, NULL);

	//clear	interrupt	status
	gpio_status	= GPIO_REG_READ(GPIO_STATUS_ADDRESS);
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);

	device.bitIdx = 0;
	device.value = 0;

	hw_timer_set_func(hwTimer);
	//os_delay_us(100);
	hw_timer_arm(650);
}

void teknetDecoderInit(uint8_t pin)
{
	device.inPin = pin;
	device.idx = 0;
	hw_timer_init(0,0);
	hw_timer_set_func(hwTimer);

	// Initialize GPO
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(pin));

	ETS_GPIO_INTR_ATTACH(irqHandler, NULL);
	gpio_pin_intr_state_set(pin, GPIO_PIN_INTR_NEGEDGE);
	ETS_GPIO_INTR_ENABLE();
}
