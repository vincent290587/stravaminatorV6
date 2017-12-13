/*
 * Vue.h
 *
 *  Created on: 12 déc. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_VUE_VUE_H_
#define SOURCE_VUE_VUE_H_

#include "ls027.h"
#include <vue/Notif.h>
#include <vue/VueCRS.h>
#include <vue/VueFEC.h>
#include <vue/VuePRC.h>
#include <vue/Menuable.h>

typedef enum {
	eVueGlobalScreenCRS,
	eVueGlobalScreenFEC,
	eVueGlobalScreenPRC,
} eVueGlobalScreenModes;

class Vue: protected VueCRS, protected VueFEC, protected VuePRC, protected NotifiableDevice, protected Menuable {
public:
	Vue();

	void init(void);

	void tasks(e_buttons_event event);

	void refresh(void);

	void drawPixel(int16_t x, int16_t y, uint16_t color);
	void cadranH(uint8_t p_lig, uint8_t nb_lig, const char *champ, String  affi, const char *p_unite);
	void cadran(uint8_t p_lig, uint8_t nb_lig, uint8_t p_col, const char *champ, String  affi, const char *p_unite);
	void clearDisplay(void) {
		LS027_Clear();
	}
	void invertDisplay(void) {
		LS027_InvertColors();
	}

private:
	eVueGlobalScreenModes m_global_mode;

	void writeWhole(void)
	{
		LS027_UpdateFull();
	}
};

#endif /* SOURCE_VUE_VUE_H_ */
