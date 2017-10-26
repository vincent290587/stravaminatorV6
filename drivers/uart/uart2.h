/*
 * uart3.h
 *
 *  Created on: 5 oct. 2017
 *      Author: Vincent
 */

#ifndef DRIVERS_UART_UART3_H_
#define DRIVERS_UART_UART3_H_

#include "fsl_uart.h"
#include "fsl_uart_edma.h"

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */


void uart2_init(uart_config_t* uartConfig);

void uart2_send(uint8_t* data, size_t length);

void uart2_wait_for_transfer();


#if defined(__cplusplus)
}
#endif /* _cplusplus */

#endif /* DRIVERS_UART_UART3_H_ */
