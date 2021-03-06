/*
 * uart2.h
 *
 *  Created on: 5 oct. 2017
 *      Author: Vincent
 */

#ifndef DRIVERS_UART_UART2_H_
#define DRIVERS_UART_UART2_H_

#include "fsl_uart.h"
#include "fsl_uart_edma.h"

#define UART2_TX_DMA_CHANNEL 5U
#define UART2_RX_DMA_CHANNEL 6U

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */


void uart2_init(uart_config_t* uartConfig);

void uart2_uninit(void);

void uart2_tasks(void);

void uart2_send(uint8_t* data, size_t length);

void uart2_wait_for_transfer();


#if defined(__cplusplus)
}
#endif /* _cplusplus */

#endif /* DRIVERS_UART_UART2_H_ */
