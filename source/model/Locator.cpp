/*
 * Locator.cpp
 *
 *  Created on: 19 oct. 2017
 *      Author: Vincent
 */

#include "segger_wrapper.h"
#include "TinyGPS++.h"
#include "parameters.h"
#include "utils.h"
#include <Locator.h>


static bool m_is_updated;
TinyGPSPlus   gps;
TinyGPSCustom hdop(gps, "GPGSA", 16);       // $GPGSA sentence, 16th element
TinyGPSCustom vdop(gps, "GPGSA", 17);       // $GPGSA sentence, 17th element
TinyGPSCustom satsInView(gps, "GPGSV", 3);  // $GPGSV sentence, third element
TinyGPSCustom satsInUse(gps, "GPGGA", 7);  // $GPGSV sentence, third element
TinyGPSCustom satNumber[4];
TinyGPSCustom snr[4];

/**
 *
 * @param c
 * @return
 */
uint32_t locator_encode_char(char c) {

	//LOG_INFO("%c", c);

	if (gps.encode(c)) {

		m_is_updated = true;

	}

	return 0;
}


Locator::Locator() {
	m_is_updated = false;

	sec_jour = 0;

	// Initialize all the uninitialized TinyGPSCustom objects
	for (int i = 0; i < 4; ++i)
	{
		satNumber[i].begin(gps, "GPGSV", 4 + 4 * i); // offsets 4, 8, 12, 16
		snr[i].begin(gps, "GPGSV", 7 + 4 * i); // offsets 7, 11, 15, 19
	}
}

/**
 *
 */
void Locator::tasks() {

	if (m_is_updated) {
		m_is_updated = false;

		if (gps.time.isUpdated()) {
			sec_jour = get_sec_jour(gps.time.hour(), gps.time.minute(), gps.time.second());
		}

		if (gps.location.isValid()) {

			m_speed    = gps.speed.kmph();
			m_altitude = gps.altitude.meters();
			gps_lat    = gps.location.lat();
			gps_lon    = gps.location.lng();

			if (snr[0].isUpdated()) {

				//LOG_INFO("\r\n%s in use vs. %s satsInView\r\n", satsInUse.value(), satsInView.value());

				for (int i = 0; i < 4; ++i)
				{
					if (satNumber[i].isValid()) {

						//LOG_INFO("Satellites signal data: sat#%s SNR= %s\r\n", satNumber[i].value(), snr[i].value());
					}
				}

			}

		}
	}
}

/**
 *
 * @return
 */
bool Locator::isUpdated()      {

	if (simu_lat.hasNewData()) {
		return true;
	}

	// NRF has newer data than GPS
	if (nrf_lon.hasNewData() &&
			gps_lat.getAge() > nrf_lat.getAge() + LNS_OVER_GPS_DTIME_MS) {
		return true;
	}

	if (gps_lon.hasNewData()) {
		return sec_jour.hasNewData();
	}

	return false;
}


/**
 *
 * @param lat
 * @param lon
 * @return
 */
int Locator::getPosition(float& lat, float& lon) {

	// pick simulation positioning first
	if (simu_lat.hasNewData()) {
		lat = simu_lat.getData();
		lon = simu_lon.getData();
		return 0;
	}

	// pick only LNS positioning if the GPS data is old
	if (nrf_lon.hasNewData() &&
				gps_lat.getAge() > nrf_lat.getAge() + LNS_OVER_GPS_DTIME_MS) {
		lat = nrf_lat.getData();
		lon = nrf_lon.getData();
		return 0;
	}

	if (gps_lon.hasNewData()) {
		lat = gps_lat.getData();
		lon = gps_lon.getData();
		return 0;
	}

	return 1;
}


/**
 *
 * @param lat
 * @param lon
 * @param sec_
 * @return
 */
int Locator::getPosition(float& lat, float& lon, uint32_t& sec_) {

	sec_ = sec_jour.getData();

	return this->getPosition(lat, lon);
}
