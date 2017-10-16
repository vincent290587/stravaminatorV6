/**
 * This is template for main module created by MCUXpresso Project Generator. Enjoy!
 **/

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "Segment.h"
#include "segger_wrapper.h"
#include "fsl_uart_edma.h"
#include "fsl_gpio.h"
#include "ls027.h"
#include "millis.h"
#include "sdcard_fatfs.h"
#include "uart0.h"
#include "uart2.h"
#include "composite.h"
#include "power_manager.h"


extern "C" int essai(void);

/*!
 * @brief Application entry point.
 */
int main(void) {
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();

	/* Define the init structure for the output LED pin*/
	gpio_pin_config_t led_config = {
			kGPIO_DigitalOutput, 0,
	};/* Init output LED GPIO. */
	GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, &led_config);

	/* Init DMAMUX */
	DMAMUX_Init(DMAMUX0);

	/* Init code */
	millis_init();

	// Segger
	segger_init();
	BOARD_InitDebugConsole();

	// USB driver
	usb_comp_init();
	CompositeTask();

	// Initialize the UART.
	uart_config_t uartConfig;
	UART_GetDefaultConfig(&uartConfig);
	uartConfig.enableTx = true;
	uartConfig.enableRx = true;

	uartConfig.baudRate_Bps = 9600U;
	uart0_init(&uartConfig);

	uartConfig.baudRate_Bps = 115200U;
	uart2_init(&uartConfig);

	// LCD driver
	LS027_Init();

	//  essai();

	delay_ms(500);

	power_manager_init();

//	power_manager_run(kAPP_PowerModeVlpr);

	GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1u << BOARD_LED_GREEN_GPIO_PIN);

	for(;;) { /* Infinite loop to avoid leaving the main function */

		delay_ms(1000); /* something to use as a breakpoint stop while looping */
		GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1u << BOARD_LED_GREEN_GPIO_PIN);

		CompositeTask();

		LOG_INFO("Hello World millis= %lu\r\n", millis());

		power_manager_run(kAPP_PowerModeRun);
		LS027_UpdateFull();
		power_manager_run(kAPP_PowerModeVlpr);

	}

	return 0;
}


