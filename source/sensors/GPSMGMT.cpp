/*
 * GPS.cpp
 *
 *  Created on: 13 déc. 2017
 *      Author: Vincent
 */

#include <GPSMGMT.h>
#include "uart0.h"
#include "utils.h"
#include "LocusCommands.h"

uint8_t buffer[30];

GPS_MGMT::GPS_MGMT() {
	// TODO Auto-generated constructor stub

}

void GPS_MGMT::standby(void) {

	const_char_to_buffer(PMTK_STANDBY, buffer, sizeof(buffer));

	uart0_send(buffer, strlen(PMTK_STANDBY));
}

void GPS_MGMT::awake(void) {

	const_char_to_buffer(PMTK_AWAKE, buffer, sizeof(buffer));

	uart0_send(buffer, strlen(PMTK_AWAKE));
}
