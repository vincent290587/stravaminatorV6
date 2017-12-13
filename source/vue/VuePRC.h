/*
 * VuePRC.h
 *
 *  Created on: 12 déc. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_VUE_VUEPRC_H_
#define SOURCE_VUE_VUEPRC_H_

#include <Adafruit_GFX.h>

typedef enum {
	eVuePRCScreenInit,
	eVuePRCScreenDataFull,
} eVuePRCScreenModes;

class VuePRC: virtual public Adafruit_GFX {
public:
	VuePRC();

	eVuePRCScreenModes tasksPRC() const;

	virtual void cadranH(uint8_t p_lig, uint8_t nb_lig, const char *champ, String  affi, const char *p_unite)=0;
	virtual void cadran(uint8_t p_lig, uint8_t nb_lig, uint8_t p_col, const char *champ, String  affi, const char *p_unite)=0;

protected:
	eVuePRCScreenModes m_prc_screen_mode;
};

#endif /* SOURCE_VUE_VUEPRC_H_ */
