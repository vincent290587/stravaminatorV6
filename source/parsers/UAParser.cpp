/*
 * UAParser.cpp
 *
 *  Created on: 13 nov. 2017
 *      Author: Vincent
 */

#include <Model.h>
#include <button.h>
#include <UAParser.h>
#include "segger_wrapper.h"

//#include "mk64f_parser.h"

void ua_send_notification_green (uint8_t on_time_) {
	nrf52_page0.neo_info.event_type = 1;
	nrf52_page0.neo_info.on_time = on_time_;
	nrf52_page0.neo_info.rgb[0] = 0x00;
	nrf52_page0.neo_info.rgb[1] = 0xFF;
	nrf52_page0.neo_info.rgb[2] = 0x00;
}

void ua_send_notification_blue (uint8_t on_time_) {
	nrf52_page0.neo_info.event_type = 1;
	nrf52_page0.neo_info.on_time = on_time_;
	nrf52_page0.neo_info.rgb[0] = 0x00;
	nrf52_page0.neo_info.rgb[1] = 0x00;
	nrf52_page0.neo_info.rgb[2] = 0xFF;
}

void ua_send_notification_red (uint8_t on_time_) {
	nrf52_page0.neo_info.event_type = 1;
	nrf52_page0.neo_info.on_time = on_time_;
	nrf52_page0.neo_info.rgb[0] = 0xFF;
	nrf52_page0.neo_info.rgb[1] = 0x00;
	nrf52_page0.neo_info.rgb[2] = 0x00;
}

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
	case k_Buttons_press_center:

		// TODO
		break;
	case k_Buttons_press_right:

		// TODO
		break;
	default:
		break;
	}

}
