/*
 * Screenutils.cpp
 *
 *  Created on: 13 déc. 2017
 *      Author: Vincent
 */

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <millis.h>
#include "WString.h"
#include "segger_wrapper.h"
#include <Screenutils.h>


String _fmkstr(float value, unsigned int nb_digits) {

	String res;

	if (fabs(value) > 100000) {
		res = "---";
		return res;
	}

	int ent_val = (int) value;
	res = String(ent_val);

	if (nb_digits > 0) {
		res += ".";
		uint16_t dec_val = fabs(value - (float)ent_val) * pow(10, nb_digits);
		res += dec_val;
	}

	return res;
}

String _imkstr(int value) {

	String res = String(value);

	return res;
}

String _secjmkstr(uint32_t value, char sep) {

	char time_char[10];

	memset(time_char, 0, sizeof(time_char));

	uint8_t hours   = (uint8_t) (value / 3600);
	value -= hours * 3600;
	uint8_t minutes = (uint8_t) (value / 60);
	value -= minutes * 60;
	uint8_t seconds = (uint8_t) (value % 60);

	(void)snprintf(time_char, sizeof(time_char), "%02u%c%02u%c%02u",
			hours, sep, minutes, sep, seconds);

	String res = time_char;

	return res;
}

String _timemkstr(SDate& date_, char sep) {

	uint32_t addition = ((millis() - date_.timestamp) / 1000.);
	uint32_t value = date_.secj;

	LOG_INFO("Addition to time: %lu\r\n", addition);

	return _secjmkstr(value + addition, sep);
}
