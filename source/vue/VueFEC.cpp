/*
 * VueFEC.cpp
 *
 *  Created on: 12 déc. 2017
 *      Author: Vincent
 */

#include <vue/VueFEC.h>

VueFEC::VueFEC() : Adafruit_GFX(0, 0) {
	m_fec_screen_mode = eVueFECScreenInit;
}

eVueFECScreenModes VueFEC::tasksFEC() const {

	// TODO affiche



	return m_fec_screen_mode;
}
