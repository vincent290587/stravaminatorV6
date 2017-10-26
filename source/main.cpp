/**
 * This is template for main module created by MCUXpresso Project Generator. Enjoy!
 **/

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "composite.h"

extern "C" void CompositeInit(void);
extern "C" void CompositeTask(void);

/*!
 * @brief Application entry point.
 */
int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

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
