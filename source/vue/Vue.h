/*
 * Vue.h
 *
 *  Created on: 12 d�c. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_VUE_VUE_H_
#define SOURCE_VUE_VUE_H_

#include "ls027.h"
#include <vue/VueCommon.h>
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

class Vue: public VueCRS, public VueFEC, public VuePRC, public NotifiableDevice, public Menuable {
public:
	Vue();

	void init(void);

	void tasks(e_buttons_event event);

	void setCurrentMode(eVueGlobalScreenModes mode_);

	void refresh(void);

	void drawPixel(int16_t x, int16_t y, uint16_t color);

	void cadran (uint8_t p_lig, uint8_t nb_lig, uint8_t p_col, const char *champ, String  affi, const char *p_unite);
	void cadranH(uint8_t p_lig, uint8_t nb_lig, const char *champ, String  affi, const char *p_unite);

	void Histo(uint8_t p_lig, uint8_t nb_lig, uint8_t p_col, sVueHistoConfiguration& h_config_);
	void HistoH (uint8_t p_lig, uint8_t nb_lig, sVueHistoConfiguration& h_config_);

	void clearDisplay(void) {
		LS027_Clear();
	}
	void invertDisplay(void) {
		LS027_InvertColors();
	}
	void writeWhole(void)
	{
		LS027_UpdateFull();
	}
private:
	eVueGlobalScreenModes m_global_mode;


};

#endif /* SOURCE_VUE_VUE_H_ */
