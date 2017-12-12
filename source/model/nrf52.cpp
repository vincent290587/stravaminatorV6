/*
 * nrf52.cpp
 *
 *  Created on: 10 déc. 2017
 *      Author: Vincent
 */

#include <nrf52.h>
#include "dma_spi0.h"
#include "segger_wrapper.h"
#include "spi_scheduler.h"
#include "parameters.h"
#include "spis_pages.h"
#include "Model.h"
#include "millis.h"

#define SPI_BAUDRATE          5000000

#define TRANSFER_SIZE         64U        /*! Transfer dataSize */

#define NRF52_CE_PIN          kDSPI_MasterPcs1
#define NRF52_CE_PIN_INIT     kDSPI_Pcs1

static uint8_t masterRxData[TRANSFER_SIZE] = {0U};
static uint8_t masterTxData[TRANSFER_SIZE] = {0U};

static spi_transfer_settings spi_settings;

static sXferTask m_spi_task;


static void _pre_transfer(void* p_context) {

	nrf52_prepare_buffer();

    return;
}

static int _start_transfer(void* p_context) {

	W_SYSVIEW_OnTaskStartExec(NRF52_TASK);

	spi_settings.masterRxData       = masterRxData;
	spi_settings.masterTxData       = masterTxData;
	spi_settings.spi_tx_data_length = TRANSFER_SIZE;

    /* Start master transfer */
	dma_spi0_transfer(&spi_settings);

    spi_settings.spi_tx_data_length = 0;

    return 0;
}

static void _post_transfer(void* p_context) {

	W_SYSVIEW_OnTaskStopExec(NRF52_TASK);

	sSpisTxInfoPage0 nrf52_data;

	nrf52_data.flags = masterRxData[TX_BUFF_FLAGS_POS];

	if (spis_decode_lns(masterRxData, &nrf52_data.lns_info)) {
		locator.nrf_loc.m_data.lat = nrf52_data.lns_info.lat;
		locator.nrf_loc.m_data.lon = nrf52_data.lns_info.lon;
		locator.nrf_loc.m_data.alt = nrf52_data.lns_info.ele;
		locator.nrf_loc.m_data.utc_time = nrf52_data.lns_info.secj;

		locator.nrf_loc.setIsUpdated();
	}

	if (spis_decode_hrm(masterRxData, &nrf52_data.hrm_info)) {
		hrm = nrf52_data.hrm_info;
	}

	if (spis_decode_bsc(masterRxData, &nrf52_data.bsc_info)) {
		cad.data.cadence = (uint16_t) nrf52_data.bsc_info.cadence;
		cad.data.speed   = ((float) nrf52_data.bsc_info.speed) / 10.;

		cad.setIsUpdated();
	}

	if (spis_decode_fec(masterRxData, &nrf52_data.fec_info)) {
		fec_info = nrf52_data.fec_info;
	}

	memset(&masterRxData, 0, sizeof(masterRxData));

    return;
}

void nrf52_init(void) {

	// fill settings
	spi_settings.configFlags        = kDSPI_MasterCtar0 | NRF52_CE_PIN;
	spi_settings.masterRxData       = masterRxData;
	spi_settings.masterTxData       = masterTxData;
	spi_settings.spi_tx_data_length = 0;

}

void nrf52_refresh(void) {

	// program the task
	memset(&m_spi_task, 0, sizeof(m_spi_task));

	m_spi_task.p_pre_func  = _pre_transfer;
	m_spi_task.p_xfer_func = _start_transfer;
	m_spi_task.p_post_func = _post_transfer;

	dma_spi0_mngr_task_add(&m_spi_task);

}

void nrf52_prepare_buffer(void) {

	sSpisRxInfo data;

	memset(&data, 0, sizeof(data));

	data.page_id = eSpiRxPage0;
	memcpy(&data.pages.page0, &nrf52_page0, sizeof(nrf52_page0));

	spis_encode_page0(&data, masterTxData);

}
