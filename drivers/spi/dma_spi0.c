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
#include "dma_spi0.h"
#include "millis.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SPI_BAUDRATE          5000000

#define DSPI_MASTER_CLK_FREQ CLOCK_GetFreq(DSPI0_CLK_SRC)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
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

volatile bool isTransferCompleted = true;

static sXferTask m_tasks[5];
static uint8_t m_tasks_nb;
static uint8_t m_cur_task;
static xferTaskState m_state;

/*******************************************************************************
 * Code
 ******************************************************************************/

void DSPI_MasterUserCallback(SPI_Type *base, dspi_master_edma_handle_t *handle, status_t status, void *userData)
{
	SEGGER_SYSVIEW_RecordEnterISR();
	if (status != kStatus_Success)
	{
		LOG_ERROR("DMA SPI0 master callback error\r\n");
	}
	isTransferCompleted = true;
//	LOG_INFO("Xfer completed %d\r\n\r\n", status);
	SEGGER_SYSVIEW_RecordExitISR();
}

void dma_spi0_uninit(void)
{

	DSPI_Deinit(SPI0);

	DMAMUX_DisableChannel(DMAMUX0, SPI0_RX_DMA_CHANNEL);
	DMAMUX_DisableChannel(DMAMUX0, SPI0_RX_DMA_CHANNEL);

}


void dma_spi0_init(void)
{

	DMAMUX_SetSource(DMAMUX0, SPI0_RX_DMA_CHANNEL, kDmaRequestMux0SPI0Rx);
	DMAMUX_EnableChannel(DMAMUX0, SPI0_RX_DMA_CHANNEL);

	DMAMUX_SetSource(DMAMUX0, SPI0_TX_DMA_CHANNEL, kDmaRequestMux0SPI0Tx);
	DMAMUX_EnableChannel(DMAMUX0, SPI0_TX_DMA_CHANNEL);

	/*DSPI init*/
	uint32_t srcClock_Hz;
	dspi_master_config_t masterConfig;

	/*Master config*/
	masterConfig.whichCtar                                = kDSPI_Ctar0;
	masterConfig.ctarConfig.baudRate                      = SPI_BAUDRATE;
	masterConfig.ctarConfig.bitsPerFrame                  = 8;
	masterConfig.ctarConfig.cpol                          = kDSPI_ClockPolarityActiveHigh;
	masterConfig.ctarConfig.cpha                          = kDSPI_ClockPhaseFirstEdge;
	masterConfig.ctarConfig.direction                     = kDSPI_LsbFirst;
	masterConfig.ctarConfig.pcsToSckDelayInNanoSec        = 1000000000 / SPI_BAUDRATE ;
	masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec    = 1000000000 / SPI_BAUDRATE ;
	masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000 / SPI_BAUDRATE ;
	masterConfig.whichPcs                                 = kDSPI_Pcs2;
	masterConfig.pcsActiveHighOrLow                       = kDSPI_PcsActiveHigh;
	masterConfig.enableContinuousSCK                      = false;
	masterConfig.enableRxFifoOverWrite                    = false;
	masterConfig.enableModifiedTimingFormat               = false;
	masterConfig.samplePoint                              = kDSPI_SckToSin0Clock;

	srcClock_Hz = DSPI_MASTER_CLK_FREQ;
	DSPI_MasterInit(SPI0, &masterConfig, srcClock_Hz);

	/* Set up dspi master */
	memset(&(dspiEdmaMasterRxRegToRxDataHandle), 0, sizeof(dspiEdmaMasterRxRegToRxDataHandle));
	memset(&(dspiEdmaMasterTxDataToIntermediaryHandle), 0, sizeof(dspiEdmaMasterTxDataToIntermediaryHandle));
	memset(&(dspiEdmaMasterIntermediaryToTxRegHandle), 0, sizeof(dspiEdmaMasterIntermediaryToTxRegHandle));

	EDMA_CreateHandle(&(dspiEdmaMasterRxRegToRxDataHandle), DMA0, SPI0_RX_DMA_CHANNEL);
	EDMA_CreateHandle(&(dspiEdmaMasterTxDataToIntermediaryHandle), DMA0, SPI0_IN_DMA_CHANNEL);
	EDMA_CreateHandle(&(dspiEdmaMasterIntermediaryToTxRegHandle), DMA0, SPI0_TX_DMA_CHANNEL);

	DSPI_MasterTransferCreateHandleEDMA(SPI0, &g_dspi_edma_m_handle, DSPI_MasterUserCallback,
			NULL, &dspiEdmaMasterRxRegToRxDataHandle,
			&dspiEdmaMasterTxDataToIntermediaryHandle,
			&dspiEdmaMasterIntermediaryToTxRegHandle);

	LOG_INFO("DMA SPI Init :-)\r\n ");
}


void dma_spi0_transfer(spi_transfer_settings* spi_settings) {

	W_SYSVIEW_OnTaskStartExec(SPI_TASK);

	/* Start master transfer */
	masterXfer.txData = spi_settings->masterTxData;
	masterXfer.rxData = spi_settings->masterRxData;
	masterXfer.dataSize = spi_settings->spi_tx_data_length;
	masterXfer.configFlags = spi_settings->configFlags | kDSPI_MasterPcsContinuous;

	// transfer the block
	status_t ret_code;
	if (kStatus_Success != (ret_code = DSPI_MasterTransferEDMA(SPI0, &g_dspi_edma_m_handle, &masterXfer)))
	{
		LOG_ERROR("DSPI_MasterTransferEDMA error: %u\r\n ", ret_code);
	} else {
		isTransferCompleted = false;
	}

	W_SYSVIEW_OnTaskStopExec(SPI_TASK);
}

void dma_spi0_mngr_init() {

	m_state = E_XFER_MNGR_IDLE;
	m_cur_task = 0;
	m_tasks_nb = 0;

	isTransferCompleted = true;
}

void dma_spi0_mngr_stop() {

	if (m_state == E_XFER_MNGR_RUN) {
		// post hook
		(*m_tasks[m_cur_task].p_post_func)(m_tasks[m_cur_task].user_data);
	}

	m_state = E_XFER_MNGR_IDLE;
	m_cur_task = 0;

	isTransferCompleted = true;
}

void dma_spi0_mngr_task_add(sXferTask *task) {

	memcpy(m_tasks+m_tasks_nb, task, sizeof(sXferTask));
	m_tasks_nb++;

	LOG_INFO("Xfer task added\r\n");

}

void dma_spi0_mngr_tasks_start() {

	if (m_state == E_XFER_MNGR_RUN) {
		dma_spi0_mngr_stop();
	}

	m_cur_task = 0;
	isTransferCompleted = true;

	if (m_tasks_nb) {
		m_state = E_XFER_MNGR_RUN;

		// start pre-hook of first task
		(*m_tasks[m_cur_task].p_pre_func)(m_tasks[m_cur_task].user_data);
	}

}

bool dma_spi0_mngr_is_running() {

	if (m_state == E_XFER_MNGR_RUN) {
		return true;
	}
	return false;
}

void dma_spi0_mngr_run() {

	// make sure we fill the DMA buffers every time with a while
	while (m_state == E_XFER_MNGR_RUN && isTransferCompleted) {

		int res = (*m_tasks[m_cur_task].p_xfer_func)(m_tasks[m_cur_task].user_data);

		if (res == 0) {
			// the task is finished
			(*m_tasks[m_cur_task].p_post_func)(m_tasks[m_cur_task].user_data);

			// check if all tasks finished
			if (m_cur_task+1 < m_tasks_nb) {

				// start of next task
				m_cur_task++;
				(*m_tasks[m_cur_task].p_pre_func)(m_tasks[m_cur_task].user_data);

			} else {
				m_state = E_XFER_MNGR_IDLE;
			}

		}

		W_SYSVIEW_OnIdle();

	}

}

void dma_spi0_mngr_finish() {

	if (m_state != E_XFER_MNGR_RUN) return;

	// TODO
//	sleep();

	// make sure we fill the DMA buffers every time with a while
	while (m_state == E_XFER_MNGR_RUN) {

		if (isTransferCompleted) dma_spi0_mngr_run();

	}

	// TODO
//	run();

}
