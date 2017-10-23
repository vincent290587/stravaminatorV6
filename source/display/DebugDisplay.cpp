/*
 * DebugDisplay.cpp
 *
 *  Created on: 20 oct. 2017
 *      Author: Vincent
 */

#include "Model.h"
#include "Segment.h"
#include "utils.h"
#include "DebugDisplay.h"
#include "segger_wrapper.h"

DebugDisplay::DebugDisplay() : SeggerDisplay() {
	// TODO Auto-generated constructor stub

}



void DebugDisplay::printSeg(uint8_t ligne, Segment& seg, uint8_t mode) {

	int posTrouve = 0;
	float minLat = 100.;
	float minLon = 400.;
	float minAlt = 10000.;
	float maxLat = -100.;
	float maxLon = -400.;
	float maxAlt = -100.;
	float curDist = 0.;
	float dDist = 0.;
	float maxDist = 0.;
	float maDist = 10000.;
	Point *pCourant, *pSuivant;
	Point *maPos;
	float maDpex = 0;
	float maDpey = 0;
	std::list<Point>::iterator _iter;
	ListePoints *liste;

	if (seg.longueur() < 4) return;

	uint16_t debut_cadran = 0;
	uint16_t fin_cadran   = SEG_LCDHEIGHT;

	zoom.setSpan(SEG_LCDWIDTH, SEG_LCDHEIGHT);

	// on cherche la taille de fenetre
	liste = seg.getListePoints();
	maPos = liste->getFirstPoint();
	pCourant = maPos;
	pSuivant = maPos;
	uint16_t points_nb = 0;
	for (_iter = liste->getLPTS()->begin(); _iter != liste->getLPTS()->end();) {

		if (seg.getStatus() == SEG_OFF && points_nb++ > SEG_OFF_NB_POINTS) {
			break;
		}

		pCourant = _iter.operator->();
		_iter++;
		if (_iter == liste->getLPTS()->end()) break;
		pSuivant = _iter.operator->();

		if (!pSuivant->isValid() || !pCourant->isValid()) break;

		if (pCourant->_lat < minLat) minLat = pCourant->_lat;
		if (pCourant->_lon < minLon) minLon = pCourant->_lon;
		if (pCourant->_alt < minAlt) minAlt = pCourant->_alt;

		if (pCourant->_lat > maxLat) maxLat = pCourant->_lat;
		if (pCourant->_lon > maxLon) maxLon = pCourant->_lon;
		if (pCourant->_alt > maxAlt) maxAlt = pCourant->_alt;

		if (pSuivant->_lat < minLat) minLat = pSuivant->_lat;
		if (pSuivant->_lon < minLon) minLon = pSuivant->_lon;
		if (pSuivant->_alt < minAlt) minAlt = pSuivant->_alt;

		if (pSuivant->_lat > maxLat) maxLat = pSuivant->_lat;
		if (pSuivant->_lon > maxLon) maxLon = pSuivant->_lon;
		if (pSuivant->_alt > maxAlt) maxAlt = pSuivant->_alt;

		if (maDist > distance_between(pCourant->_lat, pCourant->_lon, att.lat, att.lon) &&
				seg.getStatus() > SEG_OFF) {
			maDist = distance_between(pCourant->_lat, pCourant->_lon, att.lat, att.lon) + 0.1;
			maPos = pCourant;
		}

		maxDist += distance_between(pCourant->_lat, pCourant->_lon, pSuivant->_lat, pSuivant->_lon);

	}

	if (seg.getStatus() < SEG_OFF) {
		// last point
		maPos = pCourant;
	} else if (seg.getStatus() == SEG_OFF) {
		// first point
		maPos = liste->getFirstPoint();
		maDist = distance_between(maPos->_lat, maPos->_lon, att.lat, att.lon) + 0.1;

		// notre position doit etre dans le rectangle
		if (att.lat < minLat) minLat = att.lat;
		if (att.lon < minLon) minLon = att.lon;
		if (att.alt < minAlt) minAlt = att.alt;

		if (att.lat > maxLat) maxLat = att.lat;
		if (att.lon > maxLon) maxLon = att.lon;
		if (att.alt > maxAlt) maxAlt = att.alt;
	}

	// on essaye de rendre l'image carree: ratio 1:1
	// compute convertion between deg and meters
	float mdeglon_to_m = distance_between(att.lat, 0., att.lat, 0.001);
	float mdeglat_to_m = distance_between(att.lat, 0., att.lat + 0.001, 0.);

	float v_span = distance_between(minLat, minLon, maxLat, minLon);
	float h_span = distance_between(minLat, minLon, minLat, maxLon);

//	LOG_INFO("mdeglat_to_m= %d  /  mdeglon_to_m= %d\r\n", (int)mdeglat_to_m, (int)mdeglon_to_m);

	//if (seg.getStatus() != SEG_OFF) {
	if (h_span < VH_RATIO * v_span) {
		while (h_span < VH_RATIO * v_span) {
			minLon -= 50 / (1000.*mdeglon_to_m);
			maxLon += 50 / (1000.*mdeglon_to_m);
			h_span = distance_between(minLat, minLon, minLat, maxLon);
		}
	} else if (v_span < VH_RATIO * h_span) {
		while (v_span < VH_RATIO * h_span) {
			minLat -= 50 / (1000.*mdeglat_to_m);
			maxLat += 50 / (1000.*mdeglat_to_m);
			v_span = distance_between(minLat, minLon, maxLat, minLon);
		}
	}
	//}

	// marge
	minLat -= 70 / (1000.*mdeglat_to_m);
	minLon -= 70 / (1000.*mdeglon_to_m);
	minAlt -= 15.;
	maxLat += 70 / (1000.*mdeglat_to_m);
	maxLon += 70 / (1000.*mdeglon_to_m);
	maxAlt += 15.;
	maxDist += 50.;

//	LOG_INFO("vspan= %d\r\n", (int)(1000*(maxLat - minLat)));
//	LOG_INFO("hspan= %d\r\n", (int)(1000*(maxLon - minLon)));
//	LOG_INFO("point= %d-%d mins:%d-%d\r\n",
//			(int)(1000*att.lat), (int)(1000*att.lon),
//			(int)(1000*minLat), (int)(1000*minLon));

	curDist = 0.;
	// on affiche
	points_nb = 0;
	for (_iter = liste->getLPTS()->begin(); _iter != liste->getLPTS()->end(); ) {

		if (seg.getStatus() == SEG_OFF && points_nb++ > SEG_OFF_NB_POINTS) {
			break;
		}

		pCourant = _iter.operator->();
		_iter++;
		if (_iter == liste->getLPTS()->end()) break;
		pSuivant = _iter.operator->();

		if (!pSuivant->isValid() || !pCourant->isValid()) break;

		dDist = distance_between(pCourant->_lat, pCourant->_lon, pSuivant->_lat, pSuivant->_lon);

		if (mode == 0) {
			// mode segment
			drawLine(regFenLim(pCourant->_lon, minLon, maxLon, 0, SEG_LCDWIDTH),
					regFenLim(pCourant->_lat, minLat, maxLat, fin_cadran, debut_cadran),
					regFenLim(pSuivant->_lon, minLon, maxLon, 0, SEG_LCDWIDTH),
					regFenLim(pSuivant->_lat, minLat, maxLat, fin_cadran, debut_cadran), BLACK);
		} else {
			// altitude
			drawLine(regFenLim(curDist, -50., maxDist, 0, SEG_LCDWIDTH),
					regFenLim(pCourant->_alt, minAlt, maxAlt, fin_cadran, debut_cadran),
					regFenLim(curDist + dDist, -50., maxDist, 0, SEG_LCDWIDTH),
					regFenLim(pSuivant->_alt, minAlt, maxAlt, fin_cadran, debut_cadran), BLACK);
		}

		curDist += dDist;
		if (pCourant == maPos) posTrouve = 1;
		if (posTrouve == 0) {
			maDist = curDist;
		}
	}

	// draw a circle at the end of the segment
	if (seg.getStatus() != SEG_OFF) {
		drawCircle(regFenLim(pSuivant->_lon, minLon, maxLon, 0, SEG_LCDWIDTH),
				regFenLim(pSuivant->_lat, minLat, maxLat, fin_cadran, debut_cadran), 3, BLACK);
	} else {
		// draw a circle at the start of the segment
		maPos = liste->getFirstPoint();
		drawCircle(regFenLim(maPos->_lon, minLon, maxLon, 0, SEG_LCDWIDTH),
				regFenLim(maPos->_lat, minLat, maxLat, fin_cadran, debut_cadran), 3, BLACK);
	}

	// limit position when segment is finished
	float _lat, _lon, _alt;
	if (seg.getStatus() < SEG_OFF) {
		_lon = pCourant->_lon;
		_lat = pCourant->_lat;
		_alt = pCourant->_alt;
	} else {
		_lon = att.lon;
		_lat = att.lat;
		_alt = att.alt;
	}

	// ma position
	if (mode == 0) {
		// mode segment
		maDpex = regFenLim(_lon, minLon, maxLon, 0, SEG_LCDWIDTH);
		maDpey = regFenLim(_lat, minLat, maxLat, fin_cadran, debut_cadran);
	} else {
		// altitude
		maDpex = regFenLim(maDist, -50., maxDist, 0, SEG_LCDWIDTH);
		maDpey = regFenLim(_alt, minAlt, maxAlt, fin_cadran, debut_cadran);
	}
	fillCircle(maDpex, maDpey, 2, BLACK);

	// return before printing text
	if (seg.getStatus() == SEG_OFF) {
		return;
	}


//	if (maDpey > fin_cadran - 30) {
//		setCursor(maDpex > SEG_LCDWIDTH - 70 ? SEG_LCDWIDTH - 70 : maDpex, maDpey - 20);
//	} else {
//		setCursor(maDpex > SEG_LCDWIDTH - 70 ? SEG_LCDWIDTH - 70 : maDpex, maDpey + 15);
//	}

	setTextSize(1);
//	print((int)seg.getAvance());
	// completion
	setCursor(1, debut_cadran + 1);
	print((int)(maDist / curDist * 100.));
	print("%");
}

