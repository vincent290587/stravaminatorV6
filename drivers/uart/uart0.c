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
#define UART0_TX_DMA_CHANNEL 0U
#define UART0_RX_DMA_CHANNEL 1U
#define UART0_TX_DMA_REQUEST kDmaRequestMux0UART0Tx
#define UART0_RX_DMA_REQUEST kDmaRequestMux0UART0Rx

#define UART0_BUFFER_LENGTH  32

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* UART user callback */
void uart0_user_callback(void);

void uart0_callback(UART_Type *base, uart_edma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/

uart_edma_handle_t g_uartEdmaHandle;
edma_handle_t g_uartTxEdmaHandle;
edma_handle_t g_uartRxEdmaHandle;


uint8_t g_txBuffer[UART0_BUFFER_LENGTH] = {0};
uint8_t g_rxBuffer[UART0_BUFFER_LENGTH] = {0};

volatile bool rxBufferEmpty = true;
volatile bool txBufferFull = false;
volatile bool txOnGoing = false;
volatile bool rxOnGoing = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* UART user callback */
__attribute__((weak)) void uart0_user_callback(void) {
	// empty function
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
    edma_config_t config;

    UART_Init(UART0, uartConfig, UART0_CLK_FREQ);

    /* Set channel for UART */
    DMAMUX_SetSource(DMAMUX0, UART0_TX_DMA_CHANNEL, UART0_TX_DMA_REQUEST);
    DMAMUX_SetSource(DMAMUX0, UART0_RX_DMA_CHANNEL, UART0_RX_DMA_REQUEST);
    DMAMUX_EnableChannel(DMAMUX0, UART0_TX_DMA_CHANNEL);
    DMAMUX_EnableChannel(DMAMUX0, UART0_RX_DMA_CHANNEL);

    /* Init the EDMA module */
    EDMA_GetDefaultConfig(&config);
    EDMA_Init(DMA0, &config);
    EDMA_CreateHandle(&g_uartTxEdmaHandle, DMA0, UART0_TX_DMA_CHANNEL);
    EDMA_CreateHandle(&g_uartRxEdmaHandle, DMA0, UART0_RX_DMA_CHANNEL);

    /* Create UART DMA handle. */
    UART_TransferCreateHandleEDMA(UART0, &g_uartEdmaHandle, uart0_callback, NULL,
    		&g_uartTxEdmaHandle, &g_uartRxEdmaHandle);

    LOG_INFO("UART0 initialized\n");
}

void uart0_send(uint8_t* data, size_t length) {

	uart_transfer_t xfer;

    /* Send g_tipString out. */
    xfer.data = data;
    xfer.dataSize = length;

    txOnGoing = true;
    UART_SendEDMA(UART0, &g_uartEdmaHandle, &xfer);

}

void uart0_wait_for_transfer() {

    /* Wait send finished */
    while (txOnGoing) {

    }

}

void uart0_echo() {

    uart_transfer_t sendXfer;
    uart_transfer_t receiveXfer;

    /* Start to echo. */
    sendXfer.data = g_txBuffer;
    sendXfer.dataSize = UART0_BUFFER_LENGTH;
    receiveXfer.data = g_rxBuffer;
    receiveXfer.dataSize = UART0_BUFFER_LENGTH;

    while (1)
    {
        /* If RX is idle and g_rxBuffer is empty, start to read data to g_rxBuffer. */
        if ((!rxOnGoing) && rxBufferEmpty)
        {
            rxOnGoing = true;
            UART_ReceiveEDMA(UART0, &g_uartEdmaHandle, &receiveXfer);
        }

        /* If TX is idle and g_txBuffer is full, start to send data. */
        if ((!txOnGoing) && txBufferFull)
        {
            txOnGoing = true;
            UART_SendEDMA(UART0, &g_uartEdmaHandle, &sendXfer);
        }

        /* If g_txBuffer is empty and g_rxBuffer is full, copy g_rxBuffer to g_txBuffer. */
        if ((!rxBufferEmpty) && (!txBufferFull))
        {
            memcpy(g_txBuffer, g_rxBuffer, UART0_BUFFER_LENGTH);
            rxBufferEmpty = true;
            txBufferFull = true;
        }
    }
}
