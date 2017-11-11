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

	T getData() {return m_data;}

	uint32_t getLastUpdateTime() {
		return m_last_updated;
	}

	uint32_t getAge() {
		return (millis() - m_last_updated);
	}

	UData & operator=(const T &data) {
		if (m_data != data) {
			m_data = data;
			m_last_updated = millis();
		}
		return *this;
	}

protected:
	T m_data = 0;
	uint32_t m_last_updated;

};

/**
 *
 */
template <typename T>
class NData {
public:
	NData() {
		m_new_data = false;
	}

	T getData() {m_new_data = false; return m_data;}

	bool hasNewData() {
		return m_new_data;
	}

	NData & operator=(const T &data) {
		if (m_data != data) {
			m_data = data;
			m_new_data = true;
		}
		return *this;
	}

protected:
	T m_data = 0;
	bool m_new_data;

};

#endif /* SOURCE_MODEL_UDATA_H_ */
