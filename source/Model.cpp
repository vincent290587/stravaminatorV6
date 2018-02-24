/*
 * Global.cpp
 *
 *  Created on: 17 oct. 2017
 *      Author: Vincent
 */

#include "Model.h"
#include "segger_wrapper.h"

#include "composite.h"
#include "spi_scheduler.h"
#include "i2c_scheduler.h"
#include "uart0.h"


SAtt att;

Attitude      attitude;

ListeSegments mes_segments;

ListeParcours mes_parcours;

ListePoints   mes_points;

Nordic        nordic;

Locator       locator;

Boucle        boucle;

PowerManager  pwManager;

DebugDisplay  sdisplay;

Vue           vue;

UAParser      uaparser;

STC3100       stc;

VEML6075      veml;

MS5637        ms5637;

GPS_MGMT           gps_mgmt;

Sensor<sCadenceData> cad;

Sensor<sHrmInfo>     hrm;

Sensor<sFecInfo>     fec_info;

sSpisRxInfoPage0     nrf52_page0;

// init counter
int Point::objectCount = 0;

/**
 *
 */
void model_dispatch_sensors_update(void) {

	// check if backlighting is used for notifying
	if (nrf52_page0.back_info.freq == 0) {
		// setup backlight
		uint16_t light_level = veml.getRawVisComp();
		LOG_INFO("Light level: %u\r\n", light_level);
		if (light_level < BACKLIGHT_AUTO_START_RAW_VIS) {
			// il fait tout noir: TG
			nrf52_page0.back_info.freq = 0;
			nrf52_page0.back_info.state = 1;
		} else {
			// sun is shining
			nrf52_page0.back_info.freq = 0;
			nrf52_page0.back_info.state = 0;
		}
	}
}

/**
 *
 */
void perform_system_tasks(void) {

#ifndef DEBUG_CONFIG
	i2c_scheduling_tasks();
#endif

	// USB
	if (pwManager.isUsbConnected()) CompositeTask();

	gps_mgmt.tasks();

	locator.tasks();

	uaparser.tasks();

	dma_spi0_mngr_run();

	uart0_tasks();

}

/**
 *
 * @return true if memory is full, false otherwise
 */
bool check_memory_exception(void) {

	if (Point::getObjectCount() > 1500) {

		LOG_ERROR("Memory exhausted");

		return true;
	}

	return false;
}
