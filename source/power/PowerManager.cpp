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

	LED_RED_INIT(1);
	LED_BLUE_INIT(1);
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
void PowerManager::switchToRun() {
	if (m_cur_mode == kAPP_PowerModeVlpr) {

		LED_RED_OFF();
		LED_BLUE_ON();

		power_manager_run(kAPP_PowerModeRun120);
		m_cur_mode = kAPP_PowerModeRun120;

	}
}

/**
 *
 */
void PowerManager::switchToVlpr() {
	if (!this->isUsbConnected() &&
			m_cur_mode == kAPP_PowerModeRun120) {

		if (m_is_usb_init) {
			CompositeStop();
		}

		LED_RED_ON();
		LED_BLUE_OFF();

		power_manager_run(kAPP_PowerModeVlpr);
		m_cur_mode = kAPP_PowerModeVlpr;

	}
	W_SYSVIEW_OnIdle();
}
