/*
 * Cadence.H
 *
 *  Created on: 19 oct. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_MODEL_CADENCE_H_
#define SOURCE_MODEL_CADENCE_H_

#include "UData.h"
#include "stdint.h"

class Cadence {
public:
	Cadence() {}

private:
	NData<uint16_t> cadence;
	NData<float>    speed;
};


#endif /* SOURCE_MODEL_CADENCE_H_ */
