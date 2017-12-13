/*
 * BoucleFEC.cpp
 *
 *  Created on: 13 déc. 2017
 *      Author: Vincent
 */

#include <stdint.h>
#include <stdbool.h>
#include <BoucleFEC.h>
#include "Model.h"
#include "segger_wrapper.h"
#include "spi_scheduler.h"
#include "parameters.h"
#include "nrf52.h"
#include "Locator.h"


/**
 *
 */
BoucleFEC::BoucleFEC() {

}

/**
 *
 * @return true if we are ready to run
 */
bool BoucleFEC::isTime() {

	if (fec_info.getAge() >= BOUCLE_FEC_UPDATE_RATE_MS) {
		return true;
	}

	return false;
}

/**
 *
 */
void BoucleFEC::init() {

	// turn GPS OFF
	gps_mgmt.standby();

	m_needs_init = false;
}

/**
 *
 */
void BoucleFEC::run() {

	// CPU reduced speed
	pwManager.switchToRun24();

	if (m_needs_init) this->init();

	nrf52_refresh();

	dma_spi0_mngr_tasks_start();
	dma_spi0_mngr_finish();

	vue.refresh();

	dma_spi0_mngr_tasks_start();
	dma_spi0_mngr_finish();

	// go back to very low power
	pwManager.switchToVlpr();

}
