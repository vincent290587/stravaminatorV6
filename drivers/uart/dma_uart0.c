/*
 * uart0.c
 *
 *  Created on: 6 oct. 2017
 *      Author: Vincent
 */

#include "fsl_dmamux.h"
#include "segger_wrapper.h"
#include "uart0.h"
#include "fsl_dma_manager.h"
#include "GPSMGMT.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* UART instance and clock */
#define UART0_CLK_FREQ       CLOCK_GetFreq(UART0_CLK_SRC)

#define UART0_BUFFER_LENGTH  128

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* UART user callback */
void uart0_user_callback(UART_Type *base, uart_edma_handle_t *handle, status_t status, void *userData);

void uart0_callback(UART_Type *base, uart_edma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern dmamanager_handle_t dmamanager_handle;

static uart_edma_handle_t g_uartEdmaHandle;
static edma_handle_t g_uartTxEdmaHandle;
static edma_handle_t g_uartRxEdmaHandle;

static uart_transfer_t sendXfer;
static uart_transfer_t receiveXfer;

static uint32_t m_bytes_recv = 0;

static uint8_t g_rxBuffer[UART0_BUFFER_LENGTH] = {0};

volatile bool txFinished = true;
volatile bool rxFinished = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* UART callback */
void uart0_callback(UART_Type *base, uart_edma_handle_t *handle, status_t status, void *userData)
{

	if (kStatus_UART_TxIdle == status)
	{
		// TX just finished
		txFinished = true;

	} else if (kStatus_UART_RxIdle == status)
	{
		// RX just finished
		rxFinished = true;

	} else {
		LOG_INFO("UART0 status: %d\r\n", status);
	}
}

/*!
 * @brief Main function
 */
void uart0_init_(uart_config_t* uartConfig)
{
	if (kStatus_Success != UART_Init(UART0, uartConfig, UART0_CLK_FREQ)) {
		LOG_ERROR("UART0 init failure\r\n");
	}

	receiveXfer.data     = g_rxBuffer;
	receiveXfer.dataSize = UART0_BUFFER_LENGTH;

	txFinished = true;
	rxFinished = false;

	memset (g_rxBuffer, 0, sizeof(g_rxBuffer));
	memset (&g_uartTxEdmaHandle, 0, sizeof(g_uartTxEdmaHandle));
	memset (&g_uartRxEdmaHandle, 0, sizeof(g_uartRxEdmaHandle));
	memset (&g_uartEdmaHandle, 0, sizeof(g_uartEdmaHandle));

	if (kStatus_Success != DMAMGR_RequestChannel(&dmamanager_handle,
			(dma_request_source_t)kDmaRequestMux0UART0Tx,
			UART0_TX_DMA_CHANNEL, &g_uartTxEdmaHandle)) {
		LOG_ERROR("DMA channel %u occupied", UART0_TX_DMA_CHANNEL);
	}

	if (kStatus_Success != DMAMGR_RequestChannel(&dmamanager_handle,
			(dma_request_source_t)kDmaRequestMux0UART0Rx,
			UART0_RX_DMA_CHANNEL, &g_uartRxEdmaHandle)) {
		LOG_ERROR("DMA channel %u occupied", UART0_RX_DMA_CHANNEL);
	}

	/* Create UART DMA handle. */
	UART_TransferCreateHandleEDMA(UART0, &g_uartEdmaHandle, uart0_callback, NULL,
			&g_uartTxEdmaHandle, &g_uartRxEdmaHandle);

	LOG_INFO("UART0 initialized\n");

	// prepare reception
	UART_ReceiveEDMA(UART0, &g_uartEdmaHandle, &receiveXfer);
}

void uart0_uninit_()
{
	UART_Deinit(UART0);

//	DMAMUX_DisableChannel(DMAMUX0, UART0_TX_DMA_CHANNEL);
//	DMAMUX_DisableChannel(DMAMUX0, UART0_RX_DMA_CHANNEL);

	DMAMGR_ReleaseChannel(&dmamanager_handle, &g_uartTxEdmaHandle);
	DMAMGR_ReleaseChannel(&dmamanager_handle, &g_uartRxEdmaHandle);

	LOG_INFO("UART0 uninit\n");
}

void uart0_send_(uint8_t* data, size_t length) {

	/* Send g_tipString out. */
	sendXfer.data = data;
	sendXfer.dataSize = length;

	status_t ret_code;
	if (kStatus_Success != (ret_code = UART_SendEDMA(UART0, &g_uartEdmaHandle, &sendXfer))) {
		LOG_ERROR("UART_SendEDMA error %u \r\n", ret_code);
	} else {
		txFinished = false;
	}

}

void uart0_tasks_() {

//	uint32_t count=0;

//	if (kStatus_NoTransferInProgress != UART_TransferGetReceiveCountEDMA(UART0, &g_uartEdmaHandle, &count)) {
//		// handle received bytes
//		if (count) {
//
//			uint16_t ind = 0;
//			while(ind < count && ind < UART0_BUFFER_LENGTH) {
//				locator_encode_char(g_rxBuffer[ind++]);
//			}
//
//			memset (g_rxBuffer, 0, sizeof(g_rxBuffer));
//
//			// reset transfer
//			UART_TransferAbortReceiveEDMA(UART0, &g_uartEdmaHandle);
//
//			// go to next rx
//			rxFinished = true;
//		}
//	}

	if (rxFinished) {
		// buffer was full

		LOG_INFO("UART0 received data\r\n");

		uint16_t ind = 0;
		while(g_rxBuffer[ind] != 0 && ind < UART0_BUFFER_LENGTH) {
			gps_encode_char(g_rxBuffer[ind++]);
		}

		memset (g_rxBuffer, 0, sizeof(g_rxBuffer));
		m_bytes_recv = 0;

		// restart the wait for more bytes
		if (kStatus_Success == UART_ReceiveEDMA(UART0, &g_uartEdmaHandle, &receiveXfer)) {
			rxFinished = false;
		} else {
			LOG_INFO("UART0 Not ready yet\r\n");
		}
	}

}

void uart0_wait_for_transfer() {

	/* Wait send finished */
	while (!txFinished) {
		// TODO
	}

}

