/**
 * This is template for main module created by MCUXpresso Project Generator. Enjoy!
 **/

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "Segment.h"
#include "segger_wrapper.h"

#include "composite.h"
#include "power_manager.h"
#include "Model.h"

#include "dma_i2c0.h"
#include "dma_spi0.h"
#include "fsl_gpio.h"

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
		if (led_state.getAge() > 1000) {

			if (led_state.getData()) {
				led_state = false;
//				power_manager_run(kAPP_PowerModeRun);
//				LOG_INFO("RUN mode\r\n");
			} else {
				led_state = true;
//				power_manager_run(kAPP_PowerModeVlpr);
//				LOG_INFO("VLPR mode\r\n");
			}

			GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1u << BOARD_LED_GREEN_GPIO_PIN);

//			if (millis() > 15000) {
//				power_manager_run(kAPP_PowerModeVlpr);
//				LOG_INFO("VLPR mode\r\n");
//			}

			// update the screen
			lcd.setCursor(10,10);
			lcd.setTextSize(3);
			lcd.print(millis());
//			lcd.writeWhole();

//			dma_spi0_mngr_tasks_start();
//			dma_spi0_mngr_finish();

//          // test rapidite math
//			W_SYSVIEW_OnTaskStartExec(SEG_PERF_TASK);
//			for (int i = 0; i < 1000; i++) {
//				distance_between(46., ((float)i)/1000, 46., ((float)i)/1000);
//			}
//			W_SYSVIEW_OnTaskStopExec(SEG_PERF_TASK);

			W_SYSVIEW_OnIdle();
		}

		dma_spi0_mngr_run();

		locator.tasks();

	}

	return 0;
}


