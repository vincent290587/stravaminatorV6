/*
 * Locator.h
 *
 *  Created on: 19 oct. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_MODEL_LOCATOR_H_
#define SOURCE_MODEL_LOCATOR_H_

#include "UData.h"
#include <stdint.h>
#include <stdbool.h>

class Locator {
public:
	Locator();

	int getPosition(float& lat, float& lon);

	bool isUpdated() {return m_is_updated;}
	void setIsUpdated() {m_is_updated = true;}
	void clearIsUpdated() {m_is_updated = false;}

	UData<float> gps_lat;
	UData<float> gps_lon;

	UData<float> simu_lat;
	UData<float> simu_lon;

	UData<float> nrf_lat;
	UData<float> nrf_lon;

private:
	bool m_is_updated;

};

#endif /* SOURCE_MODEL_LOCATOR_H_ */
