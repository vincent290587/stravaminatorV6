/* This is a template for board specific configuration created by MCUXpresso Project Generator. Enjoy! */

#include <stdint.h>
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_gpio.h"
#include "segger_wrapper.h"
#include "composite.h"
#include "power_manager.h"

#include "millis.h"
#include "sdcard_fatfs.h"
#include "uart0.h"
#include "uart2.h"
#include "dma_spi0.h"
#include "dma_i2c0.h"

/*!
 * @brief initialize debug console to enable printf for this demo/example
 */
void BOARD_InitDebugConsole(void) {
	/* The user initialization should be placed here */
	DbgConsole_Init(BOARD_DEBUG_UART_BASEADDR, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, BOARD_DEBUG_UART_CLK_FREQ);
}

/*!
 * @brief initialize debug console to enable printf for this demo/example
 */
void BOARD_InitHardware(void) {

	// Segger
	segger_init();
	BOARD_InitDebugConsole();

	LED_GREEN_INIT(1);

	/* Init DMAMUX */
	DMAMUX_Init(DMAMUX0);

    /* Init the EDMA module */
    edma_config_t config;
    EDMA_GetDefaultConfig(&config);
    EDMA_Init(DMA0, &config);

	/* Init code */
	millis_init();

	// USB driver
	usb_comp_init();
	CompositeTask();

	// Initialize the UART.
	uart_config_t uartConfig;
	UART_GetDefaultConfig(&uartConfig);
	uartConfig.enableTx = true;
	uartConfig.enableRx = true; // TODO
	uartConfig.baudRate_Bps = 9600U;

	uart0_init(&uartConfig);

	uartConfig.baudRate_Bps = 115200U;
	uartConfig.enableRx = false; // TODO

	uart2_init(&uartConfig);

	dma_spi0_init();
	dma_spi0_mngr_init();

	dma_i2c0_init();

}
