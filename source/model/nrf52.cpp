/*
 * nrf52.cpp
 *
 *  Created on: 10 déc. 2017
 *      Author: Vincent
 */

#include <nrf52.h>
#include "assert.h"
#include "dma_spi0.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "segger_wrapper.h"
#include "spi_scheduler.h"
#include "parameters.h"
#include "spis_pages.h"
#include "Model.h"
#include "millis.h"

#define SPI_BAUDRATE          5000000

#define TRANSFER_SIZE         64U        /*! Transfer dataSize */

#define NRF52_CE_PIN          kDSPI_MasterPcs4
#define NRF52_CE_PIN_INIT     kDSPI_Pcs4

static uint8_t masterRxData[TRANSFER_SIZE] = {0U};
static uint8_t masterTxData[TRANSFER_SIZE] = {0U};

static spi_transfer_settings spi_settings;

static sXferTask m_spi_task;


static void _pre_transfer(void* p_context) {

	sSpisRxInfo data;

	memset(&data, 0, sizeof(data));
	memset(&masterTxData, 0, sizeof(masterTxData));

	data.page_id = eSpiRxPage0;
	memcpy(&data.pages.page0, &nrf52_page0, sizeof(nrf52_page0));

	spis_encode_page0(&data, masterTxData);

//	LOG_INFO("SPIS sent buffer:\r\n");
//	for (uint8_t i = 0; i < TRANSFER_SIZE; i++) {
//		LOG_INFO("%02X ", masterTxData[i]);
//	}
//	LOG_INFO("\r\n");

	// toggle CS pin manually (workaround 109 nRF52)
	GPIO_PortClear(BOARD_INITPINS_CS_NRF_GPIO, 1U << BOARD_INITPINS_CS_NRF_GPIO_PIN);
	// wait 5 us
//	delay_us(5);
	delay_ms(1);

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

	// toggle CS pin manually (workaround 109 nRF52)
	GPIO_PortSet(BOARD_INITPINS_CS_NRF_GPIO, 1U << BOARD_INITPINS_CS_NRF_GPIO_PIN);

	W_SYSVIEW_OnTaskStopExec(NRF52_TASK);

	sSpisTxInfoPage0 nrf52_data;

	LOG_INFO("SPIS recv buffer:\r\n");
	for (uint8_t i = 0; i < TRANSFER_SIZE; i++) {
		LOG_INFO("%02X ", masterRxData[i]);
	}
	LOG_INFO("\r\n");

	nrf52_data.flags = masterRxData[TX_BUFF_FLAGS_POS];

	LOG_INFO("nRF52 flags: %02X\r\n", nrf52_data.flags);

	if (nrf52_data.flags == 0xFF) {
		// handle error
		LOG_ERROR("SPI RX buffer issue");
	} else {

		if (spis_decode_lns(masterRxData, &nrf52_data.lns_info)) {
			locator.nrf_loc.data.lat = nrf52_data.lns_info.lat;
			locator.nrf_loc.data.lon = nrf52_data.lns_info.lon;
			locator.nrf_loc.data.alt = nrf52_data.lns_info.ele;
			locator.nrf_loc.data.utc_time = nrf52_data.lns_info.secj;

			LOG_INFO("LNS updated\r\n");

			locator.nrf_loc.setIsUpdated();
		}

		if (spis_decode_hrm(masterRxData, &nrf52_data.hrm_info)) {
			hrm = nrf52_data.hrm_info;

			LOG_INFO("HRM updated\r\n");
		}

		if (spis_decode_bsc(masterRxData, &nrf52_data.bsc_info)) {
			cad.data.rpm     = (uint16_t) nrf52_data.bsc_info.cadence;
			cad.data.speed   = ((float) nrf52_data.bsc_info.speed) / 10.;

			LOG_INFO("BSC updated\r\n");

			cad.setIsUpdated();
		}

		if (spis_decode_fec(masterRxData, &nrf52_data.fec_info)) {
			fec_info = nrf52_data.fec_info;

			LOG_INFO("FEC updated\r\n");
		}

	}

	memset(&masterRxData, 0, sizeof(masterRxData));

    return;
}

void nrf52_init(void) {

	assert(TRANSFER_SIZE > RX_BUFF_TOT_SIZE);
	assert(TRANSFER_SIZE > TX_BUFF_TOT_SIZE);

	// fill settings
	spi_settings.configFlags        = kDSPI_MasterCtar0 | NRF52_CE_PIN;
	spi_settings.masterRxData       = masterRxData;
	spi_settings.masterTxData       = masterTxData;
	spi_settings.spi_tx_data_length = TRANSFER_SIZE;

	gpio_pin_config_t pin_config;
	pin_config.pinDirection = kGPIO_DigitalOutput;
	pin_config.outputLogic  = 0;
	GPIO_PinInit(BOARD_INITPINS_CS_NRF_GPIO, BOARD_INITPINS_CS_NRF_GPIO_PIN, &pin_config);
}

void nrf52_refresh(void) {

	// program the task
	memset(&m_spi_task, 0, sizeof(m_spi_task));

	m_spi_task.p_pre_func  = _pre_transfer;
	m_spi_task.p_xfer_func = _start_transfer;
	m_spi_task.p_post_func = _post_transfer;

	dma_spi0_mngr_task_add(&m_spi_task);

}


