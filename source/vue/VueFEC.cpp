/*
 * VueFEC.cpp
 *
 *  Created on: 12 déc. 2017
 *      Author: Vincent
 */

#include "Model.h"
#include <vue/VueFEC.h>
#include <vue/Screenutils.h>

#define VUE_FEC_NB_LINES            5

VueFEC::VueFEC() : Adafruit_GFX(0, 0) {
	m_fec_screen_mode = eVueFECScreenInit;
}

eVueFECScreenModes VueFEC::tasksFEC() {

	eVueFECScreenModes res = m_fec_screen_mode;

	if (m_fec_screen_mode == eVueFECScreenInit) {

		// Init with welcome text
		this->setCursor(40,40);
		this->setTextSize(4);
		this->print(String("Connecting..."));

		if (fec_info.data.el_time) {
			// FEC just became active
			m_fec_screen_mode = eVueFECScreenDataFull;

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

		this->cadranH(1, VUE_FEC_NB_LINES, "Time", _timemkstr(fec_info.data.el_time), NULL);

		this->cadran(2, VUE_FEC_NB_LINES, 1, "CAD", _imkstr(cad.data.rpm), "rpm");
		this->cadran(2, VUE_FEC_NB_LINES, 2, "HRM", _imkstr(hrm.data.bpm), "bpm");

		this->cadranH(3, VUE_FEC_NB_LINES, "Speed", _fmkstr(fec_info.data.speed / 10., 1U), "km/h");

		this->cadranH(4, VUE_FEC_NB_LINES, "Pwr", _imkstr(fec_info.data.power), "W");

		this->cadran(5, VUE_FEC_NB_LINES, 1, "STC", _fmkstr(stc.getAverageCurrent(), 1U), "mA");
		this->cadran(5, VUE_FEC_NB_LINES, 2, "SOC", _imkstr(percentageBatt(stc.getVoltage(), 0.)), "%");
	}

	return res;
}
