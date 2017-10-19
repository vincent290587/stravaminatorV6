/*
 * Sensor.h
 *
 *  Created on: 19 oct. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_MODEL_SENSOR_H_
#define SOURCE_MODEL_SENSOR_H_

#include <stdbool.h>

class Sensor {
public:
	Sensor()          {m_new_data = false;}

	bool hasNewData() {return m_new_data;}

private:
	bool m_new_data;
};


#endif /* SOURCE_MODEL_SENSOR_H_ */
