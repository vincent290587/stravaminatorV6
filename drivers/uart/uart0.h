/*
 * uart0.h
 *
 *  Created on: 6 oct. 2017
 *      Author: Vincent
 */

#ifndef DRIVERS_UART_UART0_H_
#define DRIVERS_UART_UART0_H_

#include "fsl_uart.h"
#include "fsl_uart_edma.h"

#define UART0_TX_DMA_CHANNEL 3U
#define UART0_RX_DMA_CHANNEL 4U

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */


void uart0_init(uart_config_t* uartConfig);

void uart0_uninit(void);

void uart0_tasks(void);

void uart0_send(uint8_t* data, size_t length);

uint32_t uart0_get_last_rx_age(void);

void uart0_wait_for_transfer_();


#if defined(__cplusplus)
}
#endif /* _cplusplus */

#endif /* DRIVERS_UART_UART0_H_ */
