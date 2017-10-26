/*
 * PowerManager.cpp
 *
 *  Created on: 19 oct. 2017
 *      Author: Vincent
 */


#include <PowerManager.h>
#include "segger_wrapper.h"
#include "composite.h"

PowerManager::PowerManager() {

	m_is_usb_init = true;

	m_cur_mode = kAPP_PowerModeRun;
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
void PowerManager::switchToRun() {
	if (m_cur_mode == kAPP_PowerModeVlpr) {

		power_manager_run(kAPP_PowerModeRun);
		m_cur_mode = kAPP_PowerModeRun;

	}
}

/**
 *
 */
void PowerManager::switchToVlpr() {
	if (!this->isUsbConnected() &&
			m_cur_mode == kAPP_PowerModeRun) {

		if (m_is_usb_init) {
			CompositeStop();
		}

		power_manager_run(kAPP_PowerModeVlpr);
		m_cur_mode = kAPP_PowerModeVlpr;

	}
	W_SYSVIEW_OnIdle();
}
