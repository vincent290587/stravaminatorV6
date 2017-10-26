/**
 * This is template for main module created by MCUXpresso Project Generator. Enjoy!
 **/

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "composite.h"
#include "Segment.h"
#include "segger_wrapper.h"
#include "ls027.h"
#include "millis.h"
#include "sdcard_fatfs.h"

extern "C" void APPTask(void *handle);

/*!
 * @brief Application entry point.
 */
int main(void)
{
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();

	/* Init code */
	millis_init();

	// Segger
	segger_init();

	// LCD driver
	LS027_Init();

	// USB driver
//	sdcard_init();
//	sdcard_tasks();

	//essai();

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
	/* Add your code here */
	vTaskStartScheduler();

	return 0;
}
