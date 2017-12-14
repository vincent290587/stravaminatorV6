/*
 * GPS.cpp
 *
 *  Created on: 13 déc. 2017
 *      Author: Vincent
 */

#include <GPSMGMT.h>
#include "uart0.h"
#include "utils.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "LocusCommands.h"
#include "segger_wrapper.h"

uint8_t buffer[30];

GPS_MGMT::GPS_MGMT() {
	// TODO Auto-generated constructor stub

}

void GPS_MGMT::init(void) {

	// configure PPS pin
	gpio_pin_config_t pin_config;
	pin_config.outputLogic  = 0;
	pin_config.pinDirection = kGPIO_DigitalInput;
	GPIO_PinInit(BOARD_INITPINS_PPS_GPIO, BOARD_INITPINS_PPS_GPIO_PIN, &pin_config);

	// configure 3D pin
	pin_config.pinDirection = kGPIO_DigitalInput;
	GPIO_PinInit(BOARD_INITPINS_PIN_3D_GPIO, BOARD_INITPINS_PIN_3D_GPIO_PIN, &pin_config);
}

bool GPS_MGMT::is3D(void) {
	return GPIO_PinRead(BOARD_INITPINS_PIN_3D_GPIO, BOARD_INITPINS_PIN_3D_GPIO_PIN);
}

void GPS_MGMT::standby(void) {

	LOG_INFO("GPS put in standby\r\n");

	const_char_to_buffer(PMTK_STANDBY, buffer, sizeof(buffer));

	uart0_send(buffer, strlen(PMTK_STANDBY));
}

void GPS_MGMT::awake(void) {

	LOG_INFO("GPS awoken\r\n");

	const_char_to_buffer(PMTK_AWAKE, buffer, sizeof(buffer));

	uart0_send(buffer, strlen(PMTK_AWAKE));
}
