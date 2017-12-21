/*
 * VueFEC.cpp
 *
 *  Created on: 12 déc. 2017
 *      Author: Vincent
 */

#include "Model.h"
#include <vue/VueFEC.h>
#include <vue/Screenutils.h>
#include "segger_wrapper.h"

#define VUE_FEC_NB_LINES            5

VueFEC::VueFEC() : Adafruit_GFX(0, 0) {
	m_fec_screen_mode = eVueFECScreenInit;
}

eVueFECScreenModes VueFEC::tasksFEC() {

	eVueFECScreenModes res = m_fec_screen_mode;

//	if (m_el_time >= 60) {
	if (m_el_time >= 60*4 && !pwManager.isUsbConnected()) {
		nrf52_page0.power_info.state = 1;

		this->setCursor(100,50);
		this->setTextSize(3);
		this->print(String("OFF"));

		this->cadran(4, VUE_FEC_NB_LINES, 1, "Avg", _imkstr((int)stc.getAverageCurrent()), "mA");
		this->cadran(4, VUE_FEC_NB_LINES, 2, "Chrg", _fmkstr(stc.getCharge(), 1U), "mAh");

		this->cadran(5, VUE_FEC_NB_LINES, 1, "STC", _imkstr((int)stc.getCurrent()), "mA");
		this->cadran(5, VUE_FEC_NB_LINES, 2, "SOC", _imkstr(percentageBatt(stc.getVoltage(), stc.getCurrent())), "%");

		stc.shutdown();

		return eVueFECScreenDataFull;
	}

	if (m_fec_screen_mode == eVueFECScreenInit) {

		// Init with welcome text
		this->setCursor(10,50);
		this->setTextSize(3);
		this->print(String("Connecting"));

		LOG_INFO("VueFEC waiting for sensors\r\n");

		m_el_time++;

		if (fec_info.data.el_time) {
			// FEC just became active
			m_fec_screen_mode = eVueFECScreenDataFull;

			m_el_time = 0;

			// blink neopixel
			if (fec_info.data.power > 200) {
				nrf52_page0.neo_info.event_type = 1;
				nrf52_page0.neo_info.on_time = 5;
				nrf52_page0.neo_info.rgb[0] = 0x00;
				nrf52_page0.neo_info.rgb[1] = 0xFF;
				nrf52_page0.neo_info.rgb[2] = 0x00;
			}
		}

	} else if (m_fec_screen_mode == eVueFECScreenDataFull) {

		LOG_INFO("VueFEC update full data\r\n");

		this->cadranH(1, VUE_FEC_NB_LINES, "Time", _timemkstr(++m_el_time), NULL);

		this->cadran(2, VUE_FEC_NB_LINES, 1, "CAD", _imkstr(cad.data.rpm), "rpm");
		this->cadran(2, VUE_FEC_NB_LINES, 2, "HRM", _imkstr(hrm.data.bpm), "bpm");

		this->cadranH(3, VUE_FEC_NB_LINES, "Speed", _fmkstr((float)fec_info.data.speed / 10., 1U), "km/h");

		this->cadranH(4, VUE_FEC_NB_LINES, "Pwr", _imkstr(fec_info.data.power), "W");

	}

	this->cadran(4, VUE_FEC_NB_LINES, 1, "Avg", _imkstr((int)stc.getAverageCurrent()), "mA");
	this->cadran(4, VUE_FEC_NB_LINES, 2, "Chrg", _fmkstr(stc.getCharge(), 1U), "mAh");

	this->cadran(5, VUE_FEC_NB_LINES, 1, "STC", _imkstr((int)stc.getCurrent()), "mA");
	this->cadran(5, VUE_FEC_NB_LINES, 2, "SOC", _imkstr(percentageBatt(stc.getVoltage(), stc.getCurrent())), "%");

	return res;
}
