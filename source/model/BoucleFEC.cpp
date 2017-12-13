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


/**
 *
 */
BoucleFEC::BoucleFEC() {
	// TODO Auto-generated constructor stub

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

}

/**
 *
 */
void BoucleFEC::run() {

	pwManager.switchToRun120();

	nrf52_refresh();

	dma_spi0_mngr_tasks_start();

	dma_spi0_mngr_finish();

	vue.refresh();

	// go back to low power
	pwManager.switchToVlpr();
}
