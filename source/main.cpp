/**
 * This is template for main module created by MCUXpresso Project Generator. Enjoy!
 **/

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "composite.h"
#include "Segment.h"
#include "segger_wrapper.h"
#include "fsl_uart_edma.h"
#include "fsl_gpio.h"
#include "millis.h"
#include "sdcard_fatfs.h"
#include "uart0.h"
#include "uart2.h"
#include "composite.h"
#include "power_manager.h"
#include "Model.h"

extern "C" void USBTask(void *handle);

/*!
 * @brief Application task function.
 *
 * This function runs the task for application.
 *
 * @return None.
 */
void APPTask(void *handle)
{

	if (g_composite.deviceHandle)
	{
		if (xTaskCreate(USB_DeviceTask,                  /* pointer to the task */
				(char const *)"usb device task", /* task name for kernel awareness debugging */
				5000L / sizeof(portSTACK_TYPE),  /* task stack size */
				g_composite.deviceHandle,        /* optional task startup argument */
				5,                               /* initial priority */
				&g_composite.deviceTaskHandle    /* optional task handle to create */
		) != pdPASS)
		{
			usb_echo("usb device task create failed!\r\n");
			return;
		}
	}


}

/*!
 * @brief Application entry point.
 */
int main(void)
{
	BOARD_InitPins();
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

	// USB
	CompositeInit();

	// Segger
	segger_init();
	BOARD_InitDebugConsole();

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

	lcd.begin();
	// USB driver
	pwManager.init();

	boucle_crs.init();

	sdisplay.print("Hello");

	GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1u << BOARD_LED_GREEN_GPIO_PIN);

	// boucle_crs.tasks();

	if (xTaskCreate(APPTask,                           /* pointer to the task */
			(char const *)"usb device task",                         /* task name for kernel awareness debugging */
			5000L / sizeof(portSTACK_TYPE),    /* task stack size */
			0,                                 /* optional task startup argument */
			4,                                 /* initial priority */
			0                                  /* optional task handle to create */
	) != pdPASS)
	{
		usb_echo("app task create failed!\r\n");
		return 1;
	}

	vTaskStartScheduler();

	return 0;
}
