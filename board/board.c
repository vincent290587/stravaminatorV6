/* This is a template for board specific configuration created by MCUXpresso Project Generator. Enjoy! */

#include <stdint.h>
#include "fsl_debug_console.h"
#include "board.h"

/*!
 * @brief initialize debug console to enable printf for this demo/example
 */
void BOARD_InitDebugConsole(void) {
	/* The user initialization should be placed here */
	uint32_t uartClkSrcFreq = BOARD_DEBUG_UART_CLK_FREQ;
	DbgConsole_Init(BOARD_DEBUG_UART_BASEADDR, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
}

