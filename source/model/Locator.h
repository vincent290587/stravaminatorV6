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

typedef enum {
	eLocationSourceNone,
	eLocationSourceSimu,
	eLocationSourceNRF,
	eLocationSourceGPS,
} eLocationSource;

typedef struct {
	float lat;
	float lon;
	float alt;
	float speed;
	uint32_t utc_time;
} sLocationData;

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

	eLocationSource getPosition(float& lat, float& lon, uint32_t& sec_);

	eLocationSource getUpdateSource();

	bool isUpdated();

	NData<sLocationData> nrf_loc;
	NData<sLocationData> sim_loc;
	NData<sLocationData> gps_loc;

private:


};

#endif /* _cplusplus */
#endif /* SOURCE_MODEL_LOCATOR_H_ */
