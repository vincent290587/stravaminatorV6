/*
 * VuePRC.cpp
 *
 *  Created on: 12 déc. 2017
 *      Author: Vincent
 */

#include <vue/VuePRC.h>

VuePRC::VuePRC() : Adafruit_GFX(0, 0) {
	m_prc_screen_mode = eVuePRCScreenInit;
}

eVuePRCScreenModes VuePRC::tasksPRC() const {
	return m_prc_screen_mode;
}

