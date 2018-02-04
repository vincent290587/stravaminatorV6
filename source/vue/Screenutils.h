/*
 * Screenutils.h
 *
 *  Created on: 13 déc. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_VUE_SCREENUTILS_H_
#define SOURCE_VUE_SCREENUTILS_H_

#include "Attitude.h"

String _imkstr(int value);

String _fmkstr(float value, unsigned int nb_digits);

String _secjmkstr(uint32_t value, char sep);

String _timemkstr(SDate& date_, char sep);

#endif /* SOURCE_VUE_SCREENUTILS_H_ */
