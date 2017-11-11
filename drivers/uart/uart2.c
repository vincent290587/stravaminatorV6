/*
 * uart3.c
 *
 *  Created on: 5 oct. 2017
 *      Author: Vincent
 */

#include "segger_wrapper.h"
#include "uart2.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* UART instance and clock */
#define UART2_CLK_FREQ       CLOCK_GetFreq(UART2_CLK_SRC)

#define UART2_BUFFER_LENGTH  32

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* UART user callback */
extern void uart2_user_callback(UART_Type *base, uart_edma_handle_t *handle, status_t status, void *userData);

void uart2_callback(UART_Type *base, uart_edma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uart_edma_handle_t g_uartEdmaHandle;
static edma_handle_t g_uartTxEdmaHandle;
static edma_handle_t g_uartRxEdmaHandle;

static uart_transfer_t sendXfer;
static uart_transfer_t receiveXfer;

static uint32_t m_bytes_recv = 0;

static uint8_t g_rxBuffer[UART2_BUFFER_LENGTH] = {0};

volatile bool txFinished = true;
volatile bool rxFinished = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* UART callback */
void uart2_callback(UART_Type *base, uart_edma_handle_t *handle, status_t status, void *userData)
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
		LOG_INFO("UART2 status: %d\r\n", status);
	}
}

/*!
 * @brief Main function
 */
void uart2_init(uart_config_t* uartConfig)
{
	if (kStatus_Success != UART_Init(UART2, uartConfig, UART2_CLK_FREQ)) {
		LOG_ERROR("UART2 init failure\r\n");
	}

	receiveXfer.data     = g_rxBuffer;
	receiveXfer.dataSize = UART2_BUFFER_LENGTH;

	txFinished = true;
	rxFinished = false;

	memset (g_rxBuffer, 0, sizeof(g_rxBuffer));

	/* Set channel for UART */
	DMAMUX_SetSource(DMAMUX0, UART2_TX_DMA_CHANNEL, kDmaRequestMux0UART2Tx);
	DMAMUX_SetSource(DMAMUX0, UART2_RX_DMA_CHANNEL, kDmaRequestMux0UART2Rx);
	DMAMUX_EnableChannel(DMAMUX0, UART2_TX_DMA_CHANNEL);
	DMAMUX_EnableChannel(DMAMUX0, UART2_RX_DMA_CHANNEL);

	EDMA_CreateHandle(&g_uartTxEdmaHandle, DMA0, UART2_TX_DMA_CHANNEL);
	EDMA_CreateHandle(&g_uartRxEdmaHandle, DMA0, UART2_RX_DMA_CHANNEL);

	/* Create UART DMA handle. */
	UART_TransferCreateHandleEDMA(UART2, &g_uartEdmaHandle, uart2_callback, NULL,
			&g_uartTxEdmaHandle, &g_uartRxEdmaHandle);

	LOG_INFO("UART2 initialized\n");

	// prepare reception
	UART_ReceiveEDMA(UART2, &g_uartEdmaHandle, &receiveXfer);
}

void uart2_uninit()
{
	UART_Deinit(UART2);

	DMAMUX_DisableChannel(DMAMUX0, UART2_TX_DMA_CHANNEL);
	DMAMUX_DisableChannel(DMAMUX0, UART2_RX_DMA_CHANNEL);

	LOG_INFO("UART2 uninit\n");
}

void uart2_send(uint8_t* data, size_t length) {

	/* Send g_tipString out. */
	sendXfer.data = data;
	sendXfer.dataSize = length;

	status_t ret_code;
	if (kStatus_Success != (ret_code = UART_SendEDMA(UART2, &g_uartEdmaHandle, &sendXfer))) {
		LOG_ERROR("UART_SendEDMA error %u \r\n", ret_code);
	} else {
		txFinished = false;
	}

}

void uart2_tasks() {

	uint32_t count=0;

	if (kStatus_NoTransferInProgress != UART_TransferGetReceiveCountEDMA(UART2, &g_uartEdmaHandle, &count)) {
		// handle received bytes
		if (count) {
			LOG_INFO("UART2 %u new data is in the buffer !!\r\n", count - m_bytes_recv);

			LOG_INFO("->  %s\r\n", g_rxBuffer+m_bytes_recv);

			// reset transfer
			UART_TransferAbortReceiveEDMA(UART2, &g_uartEdmaHandle);

			// go to next rx
			rxFinished = true;
		}
	}

	if (rxFinished) {
		// buffer was full

		rxFinished = false;

		memset (g_rxBuffer, 0, sizeof(g_rxBuffer));
		m_bytes_recv = 0;

		// restart the wait for more bytes
		UART_ReceiveEDMA(UART2, &g_uartEdmaHandle, &receiveXfer);
	}

}

void uart2_wait_for_transfer() {

	/* Wait send finished */
	while (!txFinished) {
		// TODO
	}

}

