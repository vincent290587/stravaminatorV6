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

BoucleCRS     boucle_crs;

BoucleFEC     boucle_fec;

PowerManager  pwManager;

DebugDisplay  sdisplay;

TSharpMem     lcd;

Vue           vue;

UAParser      uaparser;

STC3100       stc;

VEML6075      veml;

MS5637        baro;

Sensor<sCadenceData> cad;

Sensor<sHrmInfo>     hrm;

Sensor<sFecInfo>     fec_info;

sSpisRxInfoPage0     nrf52_page0;
