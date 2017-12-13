/*
 * Menuable.cpp
 *
 *  Created on: 13 déc. 2017
 *      Author: Vincent
 */

#include <vue/Menuable.h>

Menuable::Menuable() {
	m_is_menu_selected = false;
}

Menuable::~Menuable() {
	// TODO Auto-generated destructor stub
}

void Menuable::propagateEvent(e_buttons_event event) {

	switch (event) {
	case k_Buttons_press_left:
		break;
	case k_Buttons_press_right:
		break;
	case k_Buttons_press_center:
		break;

	default:
		break;
	}

}

void Menuable::tasksMenu(void) {
}
