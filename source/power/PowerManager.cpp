/*
 * PowerManager.cpp
 *
 *  Created on: 19 oct. 2017
 *      Author: Vincent
 */


#include <PowerManager.h>
#include "segger_wrapper.h"
#include "composite.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_gpio.h"

PowerManager::PowerManager() {

	m_is_usb_init = true;

	m_cur_mode = kAPP_PowerModeRun120;
}

/**
 *
 */
void PowerManager::init() {
	power_manager_init();

    /* Define the init structure for the output LED pin*/
	gpio_pin_config_t pin_config;
	pin_config.outputLogic  = 0;
	pin_config.pinDirection = kGPIO_DigitalInput;
    GPIO_PinInit(BOARD_INITPINS_USB_PRES_GPIO, BOARD_INITPINS_USB_PRES_GPIO_PIN, &pin_config);

}

/**
 *
 * @return true if USB is connected
 */
bool PowerManager::isUsbConnected() {
	// TODO
#ifdef DEBUG_CONFIG
	return true;
#else

	return GPIO_PinRead(BOARD_INITPINS_USB_PRES_GPIO, BOARD_INITPINS_USB_PRES_GPIO_PIN);

#endif
}

/**
 *
 */
void PowerManager::switchToRun120() {
	if (m_cur_mode != kAPP_PowerModeRun120) {

		LED_RED_OFF();
		LED_BLUE_ON();

		power_manager_run(kAPP_PowerModeRun120);
		m_cur_mode = kAPP_PowerModeRun120;

	}
}

/**
 *
 */
void PowerManager::switchToRun24() {
	if (!this->isUsbConnected() &&
			m_cur_mode != kAPP_PowerModeRun24) {

		if (m_is_usb_init) {
			m_is_usb_init = false;
			CompositeStop();
		}

		LED_RED_OFF();

		if (power_manager_run(kAPP_PowerModeRun24)) {
			LOG_ERROR("!! switchToRun24 failed !!\r\n");
		} else {
			m_cur_mode = kAPP_PowerModeRun24;
		}

	}
}


/**
 *
 */
void PowerManager::switchToVlpr() {
	if (!this->isUsbConnected() &&
			(m_cur_mode == kAPP_PowerModeRun24 || m_cur_mode == kAPP_PowerModeRun120)) {

		if (m_is_usb_init) {
			m_is_usb_init = false;
			CompositeStop();
		}

		LED_RED_ON();
		LED_BLUE_OFF();

		power_manager_run(kAPP_PowerModeVlpr);
		m_cur_mode = kAPP_PowerModeVlpr;

	}
	W_SYSVIEW_OnIdle();
}
