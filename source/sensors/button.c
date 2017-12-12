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
#include <button.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BOARD_SW_GPIO        BOARD_INITPINS_BTN_2_GPIO
#define BOARD_SW_PORT        BOARD_INITPINS_BTN_2_PORT
#define BOARD_SW_GPIO_PIN    BOARD_INITPINS_BTN_2_GPIO_PIN
#define BOARD_SW_IRQ         PORTD_IRQn
#define BOARD_SW_IRQ_HANDLER PORTD_IRQHandler
#define BOARD_SW_NAME        BOARD_INITPINS_BTN_2_NAME

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
void BOARD_SW_IRQ_HANDLER(void)
{
    /* Clear external interrupt flag. */
    GPIO_ClearPinsInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);

    /* Change state of button. */
    g_ButtonPress++;
    g_ButtonPress_instant = millis();
}

/**
 *
 */
void buttons_init(void)
{
    /* Init input switch GPIO. */
    PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN, kPORT_InterruptRisingEdge);
    EnableIRQ(BOARD_SW_IRQ);

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
