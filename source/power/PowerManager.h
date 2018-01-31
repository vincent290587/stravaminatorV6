/*
 * PowerManager.h
 *
 *  Created on: 19 oct. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_POWER_POWERMANAGER_H_
#define SOURCE_POWER_POWERMANAGER_H_

#include "power_manager.h"
#include <stdbool.h>

class PowerManager {
public:
	PowerManager();

	void init();

	bool isUsbConnected();

	void switchToRun24();
	void switchToRun120();
	void switchToVlpr();

	void shutdown();

	app_power_mode_t getMode(void) const {
		return m_cur_mode;
	}

private:
	app_power_mode_t m_cur_mode;
	bool m_is_usb_init;
};

#endif /* SOURCE_POWER_POWERMANAGER_H_ */
