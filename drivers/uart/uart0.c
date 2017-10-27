/*
 * uart0.c
 *
 *  Created on: 6 oct. 2017
 *      Author: Vincent
 */

#include "segger_wrapper.h"
#include "uart0.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* UART instance and clock */
#define UART0_CLK_FREQ       CLOCK_GetFreq(UART0_CLK_SRC)
#define UART0_TX_DMA_REQUEST kDmaRequestMux0UART0Tx
#define UART0_RX_DMA_REQUEST kDmaRequestMux0UART0Rx

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* UART user callback */
void uart0_user_callback(void);

void uart0_callback(UART_Type *base, uart_edma_handle_t *handle, status_t status, void *userData);

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
__attribute__((weak)) void uart0_user_callback(void) {
	// TODO empty function
}

/* UART callback */
void uart0_callback(UART_Type *base, uart_edma_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_UART_TxIdle == status)
    {
        txBufferFull = false;
        txOnGoing = false;
        // TX just finished
    }

    if (kStatus_UART_RxIdle == status)
    {
        rxBufferEmpty = false;
        if (rxOnGoing == true) {
        	rxOnGoing = false;

        	// handle received string
        	uart0_user_callback();
        }

    }
}

/*!
 * @brief Main function
 */
void uart0_init(uart_config_t* uartConfig)
{
    UART_Init(UART0, uartConfig, UART0_CLK_FREQ);

    /* Set channel for UART */
    DMAMUX_SetSource(DMAMUX0, UART0_TX_DMA_CHANNEL, UART0_TX_DMA_REQUEST);
    DMAMUX_SetSource(DMAMUX0, UART0_RX_DMA_CHANNEL, UART0_RX_DMA_REQUEST);
    DMAMUX_EnableChannel(DMAMUX0, UART0_TX_DMA_CHANNEL);
    DMAMUX_EnableChannel(DMAMUX0, UART0_RX_DMA_CHANNEL);

    EDMA_CreateHandle(&g_uartTxEdmaHandle, DMA0, UART0_TX_DMA_CHANNEL);
    EDMA_CreateHandle(&g_uartRxEdmaHandle, DMA0, UART0_RX_DMA_CHANNEL);

    /* Create UART DMA handle. */
    UART_TransferCreateHandleEDMA(UART0, &g_uartEdmaHandle, uart0_callback, NULL,
    		&g_uartTxEdmaHandle, &g_uartRxEdmaHandle);

    LOG_INFO("UART0 initialized\n");
}

void uart0_uninit()
{
	UART_Deinit(UART0);

	DMAMUX_DisableChannel(DMAMUX0, UART0_TX_DMA_CHANNEL);
	DMAMUX_DisableChannel(DMAMUX0, UART0_RX_DMA_CHANNEL);

    LOG_INFO("UART0 uninit\n");
}

void uart0_send(uint8_t* data, size_t length) {

	uart_transfer_t xfer;

    /* Send g_tipString out. */
    xfer.data = data;
    xfer.dataSize = length;

    txOnGoing = true;
    status_t ret_code;
    if (kStatus_Success != (ret_code = UART_SendEDMA(UART0, &g_uartEdmaHandle, &xfer))) {
    	LOG_ERROR("UART_SendEDMA error %u \r\n", ret_code);
    }

}

void uart0_wait_for_transfer() {

    /* Wait send finished */
    while (txOnGoing) {
    	// TODO
    }

}

