/*
 * ls027.c
 *
 *  Created on: 29 sept. 2017
 *      Author: Vincent
 */

#include "spi.h"
#include "segger_wrapper.h"

//static dspi_master_handle_t g_m_handle; //global variable
static dspi_transfer_t      masterXfer;

volatile bool isSpiTransferCompleted = false;


void spi_init(dspi_master_config_t* masterConfig) {

	uint32_t srcClock_Hz = CLOCK_GetFreq(DSPI0_CLK_SRC);

	DSPI_MasterInit(SPI0, masterConfig, srcClock_Hz);

//	DSPI_MasterTransferCreateHandle(SPI0, &g_m_handle, NULL, NULL);

}



void spi_start_transfer(spi_transfer_settings* spi_settings, bool is_last_byte) {

	W_SYSVIEW_OnTaskStartExec(SPI_TASK);

    /* Start master transfer */
    masterXfer.txData = spi_settings->masterTxData;
    masterXfer.rxData = spi_settings->masterRxData;
    masterXfer.dataSize = spi_settings->spi_tx_data_length;

    // change PCS operation
    if (is_last_byte) {
    	masterXfer.configFlags = spi_settings->configFlags | kDSPI_MasterActiveAfterTransfer;
    } else {
//    	masterXfer.configFlags = spi_settings.configFlags | kDSPI_MasterActiveAfterTransfer;
    	masterXfer.configFlags = spi_settings->configFlags | kDSPI_MasterPcsContinuous;
    }

    // transfer the block
    DSPI_MasterTransferBlocking(SPI0, &masterXfer);

    spi_settings->spi_tx_data_length = 0;

    W_SYSVIEW_OnTaskStopExec(SPI_TASK);
}

