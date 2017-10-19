/*
 * Sensor.h
 *
 *  Created on: 19 oct. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_MODEL_UDATA_H_
#define SOURCE_MODEL_UDATA_H_

#include <stdint.h>
#include "millis.h"

/**
 *
 */
template <typename T>
class UData {
public:
	UData() {
		m_last_updated = 0;
	}

	T getData() const {return m_data;}

	uint32_t getLastUpdateTime() {
		return m_last_updated;
	}

	uint32_t getAge() {
		return (millis() - m_last_updated);
	}

	UData & operator=(const T &data) {
		m_data = data;
		m_last_updated = millis();
		return *this;
	}

protected:
	T m_data;
	uint32_t m_last_updated;

};


#endif /* SOURCE_MODEL_UDATA_H_ */
