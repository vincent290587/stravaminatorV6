/*
 * HRM.h
 *
 *  Created on: 12 déc. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_SENSORS_HRM_H_
#define SOURCE_SENSORS_HRM_H_

#include "UData.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

class HRM {
public:
	HRM() {}

	UData<uint8_t>  bpm;
	NData<uint16_t> rr;

private:
};

#ifdef __cplusplus
}
#endif

#endif /* SOURCE_SENSORS_HRM_H_ */
