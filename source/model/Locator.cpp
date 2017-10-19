/*
 * Locator.cpp
 *
 *  Created on: 19 oct. 2017
 *      Author: Vincent
 */

#include <Locator.h>

Locator::Locator() {
	m_is_updated = false;
}

int Locator::getPosition(float& lat, float& lon) {

	// TODO
	lat = simu_lat.getData();
	lon = simu_lon.getData();

	return 0;
}
