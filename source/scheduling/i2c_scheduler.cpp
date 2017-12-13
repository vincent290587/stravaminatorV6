/*
 * i2c_scheduler.c
 *
 *  Created on: 10 déc. 2017
 *      Author: Vincent
 */

#include <i2c_scheduler.h>
#include "segger_wrapper.h"
#include "parameters.h"
#include "millis.h"
#include "fxos.h"
#include "Model.h"

fxos_handle_t fxos_handle;

static uint32_t m_last_polled_time = 0;


void i2c_scheduling_init(void) {

	FXOS_Init(&fxos_handle);

	stc.init();

	veml.init();

	baro.init();

}

void i2c_scheduling_tasks(void) {

	if (kAPP_PowerModeVlpr == pwManager.getMode() &&
			millis() - m_last_polled_time >= SENSORS_READING_DELAY) {

		m_last_polled_time = millis();

		fxos_tasks(&fxos_handle);

		stc.refresh();

		veml.poll();

	}

}



