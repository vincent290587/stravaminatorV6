/**
 * This is template for main module created by MCUXpresso Project Generator. Enjoy!
 **/

#include "board.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_fxos.h"
#include "clock_config.h"
#include "Segment.h"
#include "segger_wrapper.h"

#include "composite.h"
#include "power_manager.h"
#include "Model.h"

#include "fxos.h"
#include "int_i2c0.h"
#include "dma_spi0.h"
#include "uart0.h"
#include "uart2.h"

/*!
 * @brief Application entry point.
 */
int main(void) {

	UData<bool> led_state;

	pwManager.init();

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitHardware();

    //
	// High level code
	//

	uaparser.init();

	// LCD driver
	lcd.begin();

//	fxos_handle_t fxos_handle;
//	FXOS_Init(&fxos_handle);

	boucle_crs.init();

	for(;;) { /* Infinite loop to avoid leaving the main function */

		// USB tasks
		if (pwManager.isUsbConnected()) CompositeTask();

		// segments tasks
		boucle_crs.tasks();

		// debug LED
		if (led_state.getAge() > 1000) {

			if (led_state.getData()) {
				led_state = false;
//				power_manager_run(kAPP_PowerModeRun120);
			} else {
				led_state = true;
//				power_manager_run(kAPP_PowerModeRun24);
			}

			LED_GREEN_TOGGLE();

//			if (millis() > 15000) {
//				power_manager_run(kAPP_PowerModeVlpr);
//				LOG_INFO("VLPR mode\r\n");
//			}

			// update the segger screen
//			sdisplay.clear();
//			sdisplay.setCursor(3,3);
//			sdisplay.setTextSize(2);
//			sdisplay.print(millis());
//			sdisplay.displayRTT();

//			lcd.setCursor(10,10);
//			lcd.setTextSize(4);
//			lcd.print(millis());
//			dma_spi0_mngr_tasks_start();
//			dma_spi0_mngr_finish();

//          // test rapidite math
//			W_SYSVIEW_OnTaskStartExec(SEG_PERF_TASK);
//			for (int i = 0; i < 1000; i++) {
//				distance_between(46., ((float)i)/1000, 46., ((float)i)/1000);
//			}
//			W_SYSVIEW_OnTaskStopExec(SEG_PERF_TASK);

//			locator.sec_jour = millis() / 1000;
//			LOG_INFO("Locator manual update\r\n");

//			uint8_t array[10] = "Hello\r\n";
//			uart2_send(array, 7);

//			fxos_tasks(&fxos_handle);

			W_SYSVIEW_OnIdle();
		}

		dma_spi0_mngr_run();

		locator.tasks();

		uaparser.tasks();

	}

	return 0;
}


