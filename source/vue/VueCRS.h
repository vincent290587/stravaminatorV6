/*
 * VueCRS.h
 *
 *  Created on: 12 déc. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_VUE_VUECRS_H_
#define SOURCE_VUE_VUECRS_H_

#include <Adafruit_GFX.h>


typedef enum {
	eVueCRSScreenInit,
	eVueCRSScreenDataFull,
	eVueCRSScreenDataSS,
	eVueCRSScreenDataDS,
} eVueCRSScreenModes;

class VueCRS: virtual public Adafruit_GFX {
public:
	VueCRS();

	eVueCRSScreenModes tasksCRS() const;

	virtual void cadranH(uint8_t p_lig, uint8_t nb_lig, const char *champ, String  affi, const char *p_unite)=0;
	virtual void cadran(uint8_t p_lig, uint8_t nb_lig, uint8_t p_col, const char *champ, String  affi, const char *p_unite)=0;

protected:
	eVueCRSScreenModes m_crs_screen_mode;
};

#endif /* SOURCE_VUE_VUECRS_H_ */
