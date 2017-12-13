/*
 * Vue.cpp
 *
 *  Created on: 12 déc. 2017
 *      Author: Vincent
 */

#include <vue/Vue.h>
#include "WString.h"


Vue::Vue() : Adafruit_GFX(LS027_HW_WIDTH, LS027_HW_HEIGHT) {

	m_global_mode = eVueGlobalScreenFEC;

}


void Vue::init(void) {
	this->setRotation(1);
	LS027_Init();
}

void Vue::tasks(e_buttons_event event) {

	// propagate to the inner menu
	this->propagateEvent(event);

}

void Vue::refresh(void) {

	if (m_is_menu_selected) {
		this->tasksMenu();
	} else {
		switch (m_global_mode) {
		case eVueGlobalScreenCRS:
			this->tasksCRS();
			break;
		case eVueGlobalScreenFEC:
			this->tasksFEC();
			break;
		case eVueGlobalScreenPRC:
			this->tasksPRC();
			break;

		default:
			break;
		}
	}

	this->writeWhole();
}

void Vue::drawPixel(int16_t x, int16_t y, uint16_t color) {

	if((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;

	switch(rotation) {
	case 1:
		adagfxswap(x, y);
		x = WIDTH  - 1 - x;
		break;
	case 2:
		x = WIDTH  - 1 - x;
		y = HEIGHT - 1 - y;
		break;
	case 3:
		adagfxswap(x, y);
		y = HEIGHT - 1 - y;
		break;
	}

	LS027_drawPixel(x, y, color);
}

void Vue::cadranH(uint8_t p_lig, uint8_t nb_lig, const char *champ, String  affi, const char *p_unite) {

	int decal = 0;
	int x = WIDTH / 2 * 0.5;
	int y = HEIGHT / nb_lig * (p_lig - 1);

	setTextColor(1,0); // 'inverted' text
	setCursor(5, y + 5);
	setTextSize(1);

	if (champ) print(champ);

	if (affi.length() < 4) {
		decal = (4 - affi.length()) * 20;
	} else if (affi.length() > 6) {
		affi = "-----";
	}

	setCursor(x + 20 + decal, y + 20);
	setTextSize(3);
	print(affi);

	setTextSize(1);
	x = WIDTH / 2;
	setCursor(x + 105, y + 5);// y + 42

	if (p_unite) print(p_unite);

	// print delimiters
	if (p_lig > 1) drawFastHLine(0, HEIGHT / nb_lig * (p_lig - 1), WIDTH, 1);
	if (p_lig < nb_lig) drawFastHLine(0, HEIGHT / nb_lig * (p_lig), WIDTH, 1);
}


void Vue::cadran(uint8_t p_lig, uint8_t nb_lig, uint8_t p_col, const char *champ, String  affi, const char *p_unite) {

	int decal = 0;
	int x = WIDTH / 2 * (p_col - 1);
	int y = HEIGHT / nb_lig * (p_lig - 1);

	setTextColor(1,0); // 'inverted' text
	setCursor(x + 5, y + 5);
	setTextSize(1);

	if (champ) print(champ);

	if (affi.length() < 6) {
		decal = (4 - affi.length()) * 14;
	} else {
		affi = "---";
	}
	setCursor(x + 25 + decal, y + 23);
	setTextSize(3);

	print(affi);

	setTextSize(1);
	setCursor(x + 95, y + 5); // y + 42

	if (p_unite) print(p_unite);

	// print delimiters
	drawFastVLine(WIDTH / 2, HEIGHT / nb_lig * (p_lig - 1), HEIGHT / nb_lig, 1);

	if (p_lig > 1) drawFastHLine(WIDTH * (p_col - 1) / 2, HEIGHT / nb_lig * (p_lig - 1), WIDTH / 2, 1);
	if (p_lig < nb_lig) drawFastHLine(WIDTH * (p_col - 1) / 2, HEIGHT / nb_lig * p_lig, WIDTH / 2, 1);
}

