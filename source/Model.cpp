/*
 * Global.cpp
 *
 *  Created on: 17 oct. 2017
 *      Author: Vincent
 */

#include "Model.h"

SAtt att;

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

MS5637        baro;

Sensor<sCadenceData> cad;

Sensor<sHrmInfo>     hrm;

Sensor<sFecInfo>     fec_info;

sSpisRxInfoPage0     nrf52_page0;

/**
 *
 */
void model_dispatch_sensors_update(void) {

	// check if backlighting is used for notifying
	if (nrf52_page0.back_info.freq == 0) {
		// setup backlight
		if (veml.getRawVisComp() < BACKLIGHT_AUTO_START_RAW_VIS) {
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
