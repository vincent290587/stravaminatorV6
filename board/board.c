/* This is a template for board specific configuration created by MCUXpresso Project Generator. Enjoy! */

#include <stdint.h>
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_debug_console.h"
#include "fsl_dma_manager.h"

#include "segger_wrapper.h"
#include "composite.h"
#include "power_manager.h"
#include "millis.h"
#include "sdcard_fatfs.h"
#include "uart0.h"
#include "uart2.h"
#include "dma_spi0.h"
#include "int_i2c0.h"

dmamanager_handle_t dmamanager_handle;

/*!
 * @brief initialize debug console to enable printf for this demo/example
 */
void BOARD_InitDebugConsole(void) {
	/* The user initialization should be placed here */
	DbgConsole_Init(0, 0, DEBUG_CONSOLE_DEVICE_TYPE_USBCDC, 0);
}

/*!
 * @brief initialize debug console to enable printf for this demo/example
 */
void BOARD_InitHardware(void) {

	// Segger
	segger_init();
	BOARD_InitDebugConsole();

	LED_GREEN_INIT(0);
	LED_RED_INIT(1);
	LED_BLUE_INIT(0);

	/* Init DMA */
	memset(&dmamanager_handle, 0, sizeof(dmamanager_handle_t));
	DMAMGR_Init(&dmamanager_handle, DMA0, 16U, 0U);

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

//	uartConfig.baudRate_Bps = 115200U;
//	uartConfig.enableRx = false; // TODO

	//uart2_init(&uartConfig);

	dma_spi0_init();

	i2c0_init();

}
