/*
 * UAParser.cpp
 *
 *  Created on: 13 nov. 2017
 *      Author: Vincent
 */

#include <Model.h>
#include <button.h>
#include <UAParser.h>

UAParser::UAParser() {

}

/**
 *
 */
void UAParser::init(void) {
	buttons_init();
}

/**
 *
 */
void UAParser::tasks(void) {

	e_buttons_event event = buttons_tasks();

	// propagate to the view
	vue.tasks(event);

	// potentially propagate to the model
	switch (event) {
	case k_Buttons_press_left:
		// TODO
		break;
	case k_Buttons_press_right:
		// TODO
		break;
	case k_Buttons_press_center:
		// TODO
		break;
	default:
		break;
	}

}
