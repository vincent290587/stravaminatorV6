/*
 * uart0.h
 *
 *  Created on: 6 oct. 2017
 *      Author: Vincent
 */

#ifndef DRIVERS_UART_UART0_H_
#define DRIVERS_UART_UART0_H_


#include "fsl_uart_edma.h"


void uart0_init(uart_config_t* uartConfig);

void uart0_send(uint8_t* data, size_t length);

void uart0_wait_for_transfer();


#endif /* DRIVERS_UART_UART0_H_ */
