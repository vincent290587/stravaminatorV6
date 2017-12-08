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
}

/**
 *
 * @return true if USB is connected
 */
bool PowerManager::isUsbConnected() {
	// TODO
	return true;
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
