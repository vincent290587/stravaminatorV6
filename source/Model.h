/*
 * Global.h
 *
 *  Created on: 17 oct. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_MODEL_H_
#define SOURCE_MODEL_H_

#include "Sensor.h"
#include "ListePoints.h"
#include "mk64f_parser.h"
#include "Segment.h"
#include "Parcours.h"
#include "Nordic.h"
#include "Locator.h"
#include "Cadence.h"
#include "Boucle.h"
#include "PowerManager.h"
#include "DebugDisplay.h"
#include "Vue.h"
#include "GPSMGMT.h"
#include "TinyGPS++.h"
#include "UAParser.h"
#include "STC3100.h"
#include "VEML6075.h"
#include "MS5637.h"
#include "parameters.h"

typedef struct {
	float lat;
	float lon;
	float alt;
	float climb;
	float vit_asc;
	int16_t pwr;
	uint16_t nbpts;
	uint8_t nbact;
	uint16_t nbsec_act;
	uint16_t next;
	uint32_t secj;
	float dist;
} SAtt;

extern SAtt att;

extern ListeSegments mes_segments;

extern ListePoints   mes_points;

extern ListeParcours mes_parcours;

extern ListePoints   mes_points;

extern Nordic        nordic;

extern Locator       locator;

extern Boucle        boucle;

extern PowerManager  pwManager;

extern DebugDisplay  sdisplay;

extern Vue           vue;

extern UAParser      uaparser;

extern STC3100       stc;

extern VEML6075      veml;

extern MS5637        baro;

extern GPS_MGMT      gps_mgmt;

extern sFecControl   fec_control;

extern Sensor<sCadenceData> cad;

extern Sensor<sHrmInfo>     hrm;

extern Sensor<sFecInfo>     fec_info;

extern sSpisRxInfoPage0     nrf52_page0;



void model_dispatch_sensors_update(void);

#endif /* SOURCE_MODEL_H_ */
