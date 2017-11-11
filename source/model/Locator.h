/*
 * Locator.h
 *
 *  Created on: 19 oct. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_MODEL_LOCATOR_H_
#define SOURCE_MODEL_LOCATOR_H_

#include <stdint.h>
#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

uint32_t locator_encode_char(char c);

#if defined(__cplusplus)
}

#include "UData.h"

/**
 *
 */
class Locator {
public:
	Locator();

	void tasks();

	int getPosition(float& lat, float& lon);
	int getPosition(float& lat, float& lon, uint32_t& sec_);

	bool isUpdated();

	NData<unsigned long int> sec_jour;

	float m_speed, m_altitude;

	UData<float> gps_lat;
	NData<float> gps_lon;

	NData<float> simu_lat;
	NData<float> simu_lon;

	UData<float> nrf_lat;
	NData<float> nrf_lon;

private:


};

#endif /* _cplusplus */
#endif /* SOURCE_MODEL_LOCATOR_H_ */
