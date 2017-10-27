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
#include "millis.h"
#include "sdcard_fatfs.h"
#include "uart0.h"
#include "uart2.h"
#include "dma_spi0.h"
#include "composite.h"
#include "power_manager.h"
#include "Model.h"



/*!
 * @brief Application entry point.
 */
int main(void) {

	UData<bool> led_state;

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

    /* Init the EDMA module */
    edma_config_t config;
    EDMA_GetDefaultConfig(&config);
    EDMA_Init(DMA0, &config);

	/* Init code */
	millis_init();

	// USB driver
	usb_comp_init();
	CompositeTask();

	// Segger
	segger_init();
	BOARD_InitDebugConsole();

	// Initialize the UART.
	uart_config_t uartConfig;
	UART_GetDefaultConfig(&uartConfig);
	uartConfig.enableTx = true;
	uartConfig.enableRx = false; // TODO

	uartConfig.baudRate_Bps = 9600U;
	uart0_init(&uartConfig);

	uartConfig.baudRate_Bps = 115200U;
	uart2_init(&uartConfig);

	pwManager.init();

	dma_spi0_init();
	dma_spi0_mngr_init();

    //
	// High level code
	//

	// LCD driver
	lcd.begin();

	boucle_crs.init();

	sdisplay.print("Hello");

	GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1u << BOARD_LED_GREEN_GPIO_PIN);

	for(;;) { /* Infinite loop to avoid leaving the main function */

		// USB tasks
		if (pwManager.isUsbConnected()) CompositeTask();

		// segments tasks
		boucle_crs.tasks();

		// debug LED
		if (led_state.getAge() > 750) {

			if (led_state.getData()) {
				led_state = false;
			} else {
				led_state = true;
			}

			GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1u << BOARD_LED_GREEN_GPIO_PIN);

		}
			lcd.setCursor(10,10);
			lcd.setTextSize(3);
			lcd.print(millis());
			dma_spi0_mngr_finish();

			dma_spi0_mngr_tasks_start();
			W_SYSVIEW_OnIdle();

		dma_spi0_mngr_run();
	}

	return 0;
}


