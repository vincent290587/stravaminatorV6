/*
 * dma_spi0.c
 *
 *  Created on: 26 oct. 2017
 *      Author: Vincent
 */


#include "fsl_device_registers.h"
#include "segger_wrapper.h"
#include "fsl_dspi.h"
#include "fsl_dspi_edma.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"
#include "fsl_dma_manager.h"
#include "dma_spi0.h"
#include "spi_scheduler.h"
#include "millis.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SPI_BAUDRATE          2000000

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern dmamanager_handle_t dmamanager_handle;

/* DSPI user callback */
void DSPI_MasterUserCallback(SPI_Type *base, dspi_master_edma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/

dspi_master_edma_handle_t g_dspi_edma_m_handle;

edma_handle_t dspiEdmaMasterRxRegToRxDataHandle;
edma_handle_t dspiEdmaMasterTxDataToIntermediaryHandle;
edma_handle_t dspiEdmaMasterIntermediaryToTxRegHandle;

dspi_transfer_t masterXfer;

dspi_master_config_t masterConfig;

volatile bool isSpiTransferCompleted = true;



/*******************************************************************************
 * Code
 ******************************************************************************/

void DSPI_MasterUserCallback(SPI_Type *base, dspi_master_edma_handle_t *handle, status_t status, void *userData)
{
	W_SYSVIEW_RecordEnterISR();
	if (status != kStatus_Success)
	{
		LOG_ERROR("DMA SPI0 master callback error\r\n");
	}

	isSpiTransferCompleted = true;

	dma_spi0_mngr_callback();

	W_SYSVIEW_RecordExitISR();
}

void dma_spi0_uninit(void)
{
	DSPI_MasterTransferAbortEDMA(SPI0, &g_dspi_edma_m_handle);

	DSPI_Deinit(SPI0);

	delay_ms(1);

	DMAMGR_ReleaseChannel(&dmamanager_handle, &dspiEdmaMasterRxRegToRxDataHandle);
	DMAMGR_ReleaseChannel(&dmamanager_handle, &dspiEdmaMasterTxDataToIntermediaryHandle);
	DMAMGR_ReleaseChannel(&dmamanager_handle, &dspiEdmaMasterIntermediaryToTxRegHandle);
}


void dma_spi0_update_clocks(void)
{
    if (0 == DSPI_MasterSetBaudRate(SPI0, masterConfig.whichCtar, masterConfig.ctarConfig.baudRate, CLOCK_GetFreq(DSPI0_CLK_SRC)))
    {
    	LOG_ERROR("DSPI_MasterSetBaudRate error");
        assert(false);
    }

    DSPI_MasterSetDelayTimes(SPI0, masterConfig.whichCtar, kDSPI_PcsToSck, CLOCK_GetFreq(DSPI0_CLK_SRC),
                             masterConfig.ctarConfig.pcsToSckDelayInNanoSec);
    DSPI_MasterSetDelayTimes(SPI0, masterConfig.whichCtar, kDSPI_LastSckToPcs, CLOCK_GetFreq(DSPI0_CLK_SRC),
                             masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec);
    DSPI_MasterSetDelayTimes(SPI0, masterConfig.whichCtar, kDSPI_BetweenTransfer, CLOCK_GetFreq(DSPI0_CLK_SRC),
                             masterConfig.ctarConfig.betweenTransferDelayInNanoSec);
}


void dma_spi0_init(void)
{

	/* Set up dspi master */
	memset(&(dspiEdmaMasterRxRegToRxDataHandle), 0, sizeof(dspiEdmaMasterRxRegToRxDataHandle));
	memset(&(dspiEdmaMasterTxDataToIntermediaryHandle), 0, sizeof(dspiEdmaMasterTxDataToIntermediaryHandle));
	memset(&(dspiEdmaMasterIntermediaryToTxRegHandle), 0, sizeof(dspiEdmaMasterIntermediaryToTxRegHandle));

	if (kStatus_Success != DMAMGR_RequestChannel(&dmamanager_handle,
			(dma_request_source_t)kDmaRequestMux0SPI0Rx,
			SPI0_RX_DMA_CHANNEL, &dspiEdmaMasterRxRegToRxDataHandle)) {
		LOG_ERROR("DMA channel %u occupied", SPI0_RX_DMA_CHANNEL);
	}

	if (kStatus_Success != DMAMGR_RequestChannel(&dmamanager_handle,
			(dma_request_source_t)kDmaRequestMux0SPI0Tx,
			SPI0_TX_DMA_CHANNEL, &dspiEdmaMasterIntermediaryToTxRegHandle)) {
		LOG_ERROR("DMA channel %u occupied", SPI0_TX_DMA_CHANNEL);
	}

	if (kStatus_Success != DMAMGR_RequestChannel(&dmamanager_handle,
			0,
			SPI0_IN_DMA_CHANNEL, &dspiEdmaMasterTxDataToIntermediaryHandle)) {
		LOG_ERROR("DMA channel %u occupied", SPI0_IN_DMA_CHANNEL);
	}


	/*Master config*/
	masterConfig.whichCtar                                = kDSPI_Ctar0;
	masterConfig.ctarConfig.baudRate                      = SPI_BAUDRATE;
	masterConfig.ctarConfig.bitsPerFrame                  = 8;
	masterConfig.ctarConfig.cpol                          = kDSPI_ClockPolarityActiveHigh;
	masterConfig.ctarConfig.cpha                          = kDSPI_ClockPhaseFirstEdge;
	masterConfig.ctarConfig.direction                     = kDSPI_LsbFirst;
	masterConfig.ctarConfig.pcsToSckDelayInNanoSec        = 300 ;
	masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec    = 300 ;
	masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 300 ;
	masterConfig.whichPcs                                 = kDSPI_Pcs2;
	masterConfig.pcsActiveHighOrLow                       = kDSPI_PcsActiveHigh;
	masterConfig.enableContinuousSCK                      = false;
	masterConfig.enableRxFifoOverWrite                    = false;
	masterConfig.enableModifiedTimingFormat               = false;
	masterConfig.samplePoint                              = kDSPI_SckToSin0Clock;

	DSPI_MasterInit(SPI0, &masterConfig, CLOCK_GetFreq(DSPI0_CLK_SRC));

//	DSPI_SetAllPcsPolarity(SPI0, kDSPI_Pcs1ActiveLow);

	DSPI_MasterTransferCreateHandleEDMA(SPI0, &g_dspi_edma_m_handle, DSPI_MasterUserCallback,
			NULL, &dspiEdmaMasterRxRegToRxDataHandle,
			&dspiEdmaMasterTxDataToIntermediaryHandle,
			&dspiEdmaMasterIntermediaryToTxRegHandle);

	LOG_INFO("DMA SPI Init\r\n");
}


void dma_spi0_transfer(spi_transfer_settings* spi_settings) {

	/* Start master transfer */
	masterXfer.txData = spi_settings->masterTxData;
	masterXfer.rxData = spi_settings->masterRxData;
	masterXfer.dataSize = spi_settings->spi_tx_data_length;
	masterXfer.configFlags = spi_settings->configFlags | kDSPI_MasterPcsContinuous;

	dma_spi0_finish_transfer();

	// transfer the block
	status_t ret_code;
	if (kStatus_Success != (ret_code = DSPI_MasterTransferEDMA(SPI0, &g_dspi_edma_m_handle, &masterXfer)))
	{
		LOG_ERROR("DSPI_MasterTransferEDMA error: %u\r\n ", ret_code);
	} else {
		isSpiTransferCompleted = false;
	}

}

void dma_spi0_finish_transfer(void) {

	while (!isSpiTransferCompleted) {
		// sleep
		sleep();
	}

}
