/*
 * Zoom.cpp
 *
 *  Created on: 14 oct. 2017
 *      Author: Vincent
 */

#include "Zoom.h"
#include "utils.h"

Zoom::Zoom() {
	m_h_size = 80;
	m_v_size = 30;

	m_zoom_level = 1;
}

void Zoom::decreaseZoom() {
	if (m_zoom_level < 100) m_zoom_level += 1;
}

void Zoom::increaseZoom() {
	if (m_zoom_level > 0) m_zoom_level -= 1;
}

void Zoom::resetZoom() {
	m_zoom_level = BASE_ZOOM_LEVEL;
}

void Zoom::setSpan(uint16_t h_space, uint16_t v_space) {
	m_h_size = h_space;
	m_v_size = v_space;
}

/**
 *  * Zoom horizontal level=5   : m_h_size/2 pixels = 60m
 *                    level=6   : m_h_size/2 pixels = 72m
 *
 *                                |
 *              *------h_zoom-----|
 *                                |
 *
 * @param lat Current latitude
 * @param distance Distance to the closest point
 * @param h_zoom Longitude half-span to be displayed
 * @param v_zoom Latitude half-span to be displayed
 */
void Zoom::computeZoom(float lat, float distance, float& h_zoom, float& v_zoom) {

	// compute convertion between mdeg and meters
	float deglon_to_m = 1000. * distance_between(lat, 0., lat, 0.001);
	float deglat_to_m = 1000. * distance_between(lat, 0., lat + 0.001, 0.);

	// worst case of horizontal distance
	float distance_h = distance * 1.414;

	float h_zoom_m = m_zoom_level * BASE_ZOOM_METERS / BASE_ZOOM_LEVEL;

	// make sure the PRC is in the screen
	if (h_zoom_m < distance_h) {
		h_zoom_m = 1.3 * distance_h;
	}

	// convert to degrees
	h_zoom = h_zoom_m / deglon_to_m;

	// rule of 3 for the vertical
	float v_zoom_m = h_zoom_m * m_v_size / m_h_size;
	v_zoom = v_zoom_m / deglat_to_m;

}
