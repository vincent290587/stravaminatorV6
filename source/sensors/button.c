/*
 * button.cpp
 *
 *  Created on: 13 nov. 2017
 *      Author: Vincent
 */

#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "board.h"
#include "pin_mux.h"
#include "millis.h"
#include "segger_wrapper.h"
#include <button.h>


/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Whether the SW button is pressed */
volatile uint8_t g_ButtonPress          = 0;
volatile uint32_t g_ButtonPress_instant = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Interrupt service fuction of switch.
 *
 * This function toggles the LED
 */
void PORTD_IRQHandler(void)
{
    /* Clear external interrupt flag. */
//	uint32_t flags = GPIO_PortGetInterruptFlags(BOARD_INITPINS_BTN_2_GPIO);

	GPIO_PortClearInterruptFlags(BOARD_INITPINS_BTN_2_GPIO, 1U << BOARD_INITPINS_BTN_2_GPIO_PIN);

    /* Change state of button. */
    g_ButtonPress++;
    g_ButtonPress_instant = millis();

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

/**
 *
 */
void buttons_init(void)
{
    /* Init input switch GPIO. */
    PORT_SetPinInterruptConfig(BOARD_INITPINS_BTN_2_PORT, BOARD_INITPINS_BTN_2_GPIO_PIN, kPORT_InterruptRisingEdge);
    EnableIRQ(PORTD_IRQn);

    /* Define the init structure for the output LED pin*/
	gpio_pin_config_t pin_config;
	pin_config.outputLogic  = 0;
	pin_config.pinDirection = kGPIO_DigitalInput;
    GPIO_PinInit(BOARD_INITPINS_BTN_2_GPIO, BOARD_INITPINS_BTN_2_GPIO_PIN, &pin_config);

    g_ButtonPress         = 0;
    g_ButtonPress_instant = 0;
}

/**
 *
 * @return
 */
e_buttons_event buttons_tasks(void) {

	e_buttons_event res = k_Buttons_no_event;

	if (g_ButtonPress_instant &&
			(millis() - g_ButtonPress_instant > 2)) {

		res = g_ButtonPress;

		g_ButtonPress         = 0;
		g_ButtonPress_instant = 0;
	}

	return res;
}
