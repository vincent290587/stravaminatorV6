/*
 * VueCRS.cpp
 *
 *  Created on: 12 déc. 2017
 *      Author: Vincent
 */

#include <vue/VueCRS.h>

VueCRS::VueCRS() : Adafruit_GFX(0, 0) {
}

eVueCRSScreenModes VueCRS::tasksCRS() const {
	return m_crs_screen_mode;
}
