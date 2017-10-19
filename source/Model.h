/*
 * Global.h
 *
 *  Created on: 17 oct. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_MODEL_H_
#define SOURCE_MODEL_H_

#include "UData.h"
#include "ListePoints.h"
#include "Segment.h"
#include "Parcours.h"
#include "Nordic.h"
#include "Locator.h"
#include "BoucleCRS.h"
#include "PowerManager.h"

typedef struct {
	float lat;
	float lon;
} SAtt;

extern SAtt att;

extern ListeSegments mes_segments;

extern ListePoints   mes_points;

extern ListeParcours mes_parcours;

extern ListePoints   mes_points;

extern Nordic        nordic;

extern Locator       locator;

extern BoucleCRS     boucle_crs;

extern PowerManager  pwManager;

#endif /* SOURCE_MODEL_H_ */
