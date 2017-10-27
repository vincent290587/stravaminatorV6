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

#define UART0_TX_DMA_CHANNEL 0U
#define UART0_RX_DMA_CHANNEL 1U

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */


void uart0_init(uart_config_t* uartConfig);

void uart0_uninit(void);

void uart0_send(uint8_t* data, size_t length);

void uart0_wait_for_transfer();


#if defined(__cplusplus)
}
#endif /* _cplusplus */

#endif /* DRIVERS_UART_UART0_H_ */
