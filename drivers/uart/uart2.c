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
#define UART2_TX_DMA_CHANNEL 2U
#define UART2_RX_DMA_CHANNEL 3U
#define UART2_TX_DMA_REQUEST kDmaRequestMux0UART2Tx
#define UART2_RX_DMA_REQUEST kDmaRequestMux0UART2Rx

#define UART2_BUFFER_LENGTH  32

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* UART user callback */
void uart2_user_callback(void);

void uart2_callback(UART_Type *base, uart_edma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uart_edma_handle_t g_uartEdmaHandle;
static edma_handle_t g_uartTxEdmaHandle;
static edma_handle_t g_uartRxEdmaHandle;


volatile bool rxBufferEmpty = true;
volatile bool txBufferFull = false;
volatile bool txOnGoing = false;
volatile bool rxOnGoing = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* UART user callback */
__attribute__((weak)) void uart2_user_callback(void) {
	// empty function
}

/* UART callback */
void uart2_callback(UART_Type *base, uart_edma_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_UART_TxIdle == status)
    {
        txBufferFull = false;
        txOnGoing = false;
        // TX just finished
        LOG_INFO("UART2 TX just finished\n");
    }

    if (kStatus_UART_RxIdle == status)
    {
        rxBufferEmpty = false;
        if (rxOnGoing == true) {
        	rxOnGoing = false;

        	LOG_INFO("UART2 RX just finished\n");

        	// handle received string
        	uart2_user_callback();
        }

    }
}

/*!
 * @brief Main function
 */
void uart2_init(uart_config_t* uartConfig)
{
    UART_Init(UART2, uartConfig, UART2_CLK_FREQ);

    /* Set channel for UART */
    DMAMUX_SetSource(DMAMUX0, UART2_TX_DMA_CHANNEL, UART2_TX_DMA_REQUEST);
    DMAMUX_SetSource(DMAMUX0, UART2_RX_DMA_CHANNEL, UART2_RX_DMA_REQUEST);
    DMAMUX_EnableChannel(DMAMUX0, UART2_TX_DMA_CHANNEL);
    DMAMUX_EnableChannel(DMAMUX0, UART2_RX_DMA_CHANNEL);

    /* Init the EDMA module */
    EDMA_CreateHandle(&g_uartTxEdmaHandle, DMA0, UART2_TX_DMA_CHANNEL);
    EDMA_CreateHandle(&g_uartRxEdmaHandle, DMA0, UART2_RX_DMA_CHANNEL);

    /* Create UART DMA handle. */
    UART_TransferCreateHandleEDMA(UART2, &g_uartEdmaHandle, uart2_callback, NULL,
    		&g_uartTxEdmaHandle, &g_uartRxEdmaHandle);

    LOG_INFO("UART2 initialized\n");
}

void uart2_uninit()
{
	UART_Deinit(UART2);

	DMAMUX_DisableChannel(DMAMUX0, UART2_TX_DMA_CHANNEL);
	DMAMUX_DisableChannel(DMAMUX0, UART2_RX_DMA_CHANNEL);

    LOG_INFO("UART2 uninit\n");
}

void uart2_send(uint8_t* data, size_t length) {

	uart_transfer_t xfer;

    /* Send g_tipString out. */
    xfer.data = data;
    xfer.dataSize = length;

    txOnGoing = true;
    status_t ret_code;
    if (kStatus_Success != (ret_code = UART_SendEDMA(UART2, &g_uartEdmaHandle, &xfer))) {
    	LOG_ERROR("UART_SendEDMA error %u \r\n", ret_code);
    }

}

void uart2_wait_for_transfer() {

    /* Wait send finished */
    while (txOnGoing) {

    }

}

