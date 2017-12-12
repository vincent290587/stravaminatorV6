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

	// Initialize all the uninitialized TinyGPSCustom objects
	for (int i = 0; i < 4; ++i)
	{
		satNumber[i].begin(gps, "GPGSV", 4 + 4 * i); // offsets 4, 8, 12, 16
		snr[i].begin(gps, "GPGSV", 7 + 4 * i); // offsets 7, 11, 15, 19
	}
}

/**
 *
 * @return
 */
eLocationSource Locator::getUpdateSource() {

	if (sim_loc.hasNewData()) {
		return eLocationSourceSimu;
	}

	if (gps_loc.hasNewData()) {
		return eLocationSourceGPS;
	}

	// NRF has newer data than GPS
	if (nrf_loc.hasNewData() &&
			nrf_loc.m_data.utc_time > gps_loc.m_data.utc_time + LNS_OVER_GPS_DTIME_S) {
		return eLocationSourceNRF;
	}

	return eLocationSourceNone;
}

/**
 *
 * @return
 */
bool Locator::isUpdated()      {

	if (eLocationSourceNone != this->getUpdateSource()) {
		return true;
	}
	return false;
}

/**
 *
 * @param lat
 * @param lon
 * @param sec_
 * @return
 */
eLocationSource Locator::getPosition(float& lat, float& lon, uint32_t& sec) {

	eLocationSource res = this->getUpdateSource();

	switch (res) {
	case eLocationSourceSimu:
	{
		sLocationData& data = sim_loc.getData();
		lat = data.lat;
		lon = data.lon;
		sec = data.utc_time;
	}
	break;
	case eLocationSourceNRF:
	{
		sLocationData& data = nrf_loc.getData();
		lat = data.lat;
		lon = data.lon;
		sec = data.utc_time;
	}
	break;
	case eLocationSourceGPS:
	{
		sLocationData& data = gps_loc.getData();
		lat = data.lat;
		lon = data.lon;
		sec = data.utc_time;
	}
	break;
	case eLocationSourceNone:
	default:
		break;
	}

	return res;
}

/**
 * Used to get the data from the GPS parsing module
 */
void Locator::tasks() {

	if (m_is_updated) {
		m_is_updated = false;

		if (gps.time.isUpdated()) {
			gps_loc.m_data.utc_time = get_sec_jour(gps.time.hour(), gps.time.minute(), gps.time.second());
		}

		if (gps.location.isValid()) {

			gps_loc.m_data.speed  = gps.speed.kmph();
			gps_loc.m_data.alt    = gps.altitude.meters();
			gps_loc.m_data.lat    = gps.location.lat();
			gps_loc.m_data.lon    = gps.location.lng();

			gps_loc.setIsUpdated();

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
