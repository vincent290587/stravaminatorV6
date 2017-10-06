/**
 * This is template for main module created by MCUXpresso Project Generator. Enjoy!
 **/

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "Segment.h"
#include "segger_wrapper.h"
#include "fsl_uart_edma.h"
#include "ls027.h"
#include "millis.h"
#include "sdcard_fatfs.h"
#include "uart0.h"
#include "uart2.h"

extern "C" int essai(void);

/*!
 * @brief Application entry point.
 */
int main(void) {
  /* Init board hardware. */
  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  BOARD_InitDebugConsole();

  /* Init DMAMUX */
  DMAMUX_Init(DMAMUX0);

  /* Init code */
  millis_init();

  // Segger
  segger_init();

  // LCD driver
  LS027_Init();

  // Initialize the UART.
  uart_config_t uartConfig;
  UART_GetDefaultConfig(&uartConfig);
  uartConfig.enableTx = true;
  uartConfig.enableRx = true;

  uartConfig.baudRate_Bps = 9600U;
  uart0_init(&uartConfig);

  uartConfig.baudRate_Bps = 115200U;
  uart2_init(&uartConfig);

  // USB driver
  sdcard_init();
  sdcard_tasks();

  essai();

  for(;;) { /* Infinite loop to avoid leaving the main function */

	delay_ms(100); /* something to use as a breakpoint stop while looping */

	sdcard_tasks();

    LOG_INFO("Hello World millis= %lu\r\n", millis());

    LS027_UpdateFull();

  }

  return 0;
}


