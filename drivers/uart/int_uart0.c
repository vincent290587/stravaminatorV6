/*
 * int_uart0.c
 *
 *  Created on: 11 nov. 2017
 *      Author: Vincent
 */


/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define USE_RTT 0

#include "board.h"
#include "fsl_uart.h"
#include "segger_wrapper.h"
#include "locator.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* UART clock */
#define UART_CLK_FREQ CLOCK_GetFreq(UART0_CLK_SRC)

#define UART0_RB_SIZE         256

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t uart0_rb[UART0_RB_SIZE];
volatile uint16_t txIndex; /* Index of the data to send out. */
volatile uint16_t rxIndex; /* Index of the memory to save new arrived data. */

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 *
 */
void UART0_RX_TX_IRQHandler(void)
{
    uint8_t data;

    W_SYSVIEW_RecordEnterISR();

    /* If new data arrived. */
    if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(UART0))
    {
        data = UART_ReadByte(UART0);

        /* If ring buffer is not full, add data to ring buffer. */
        if (((rxIndex + 1) % UART0_RB_SIZE) != txIndex)
        {
        	uart0_rb[rxIndex] = data;
            rxIndex++;
            rxIndex %= UART0_RB_SIZE;
        }
    }

    W_SYSVIEW_RecordExitISR();
}

/**
 *
 * @param uartConfig
 */
void uart0_init(uart_config_t* config)
{

    UART_Init(UART0, config, UART_CLK_FREQ);

    /* Enable RX interrupt. */
    UART_EnableInterrupts(UART0, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
    EnableIRQ(UART0_RX_TX_IRQn);

    txIndex = 0;
    rxIndex = 0;

}


void uart0_uninit(void)
{
    /* Disable RX interrupt. */
	UART_DisableInterrupts(UART0, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
	DisableIRQ(UART0_RX_TX_IRQn);

	UART_Deinit(UART0);
}

void uart0_send(uint8_t* data, size_t length) {

	UART_WriteBlocking(UART0, data, length);

}


void uart0_tasks() {

    /* If ring buffer is not empty, parse data. */
    while (((txIndex + 0) % UART0_RB_SIZE) != rxIndex)
    {
    	locator_encode_char(uart0_rb[txIndex]);

    	txIndex++;
    	txIndex %= UART0_RB_SIZE;
    }

}
