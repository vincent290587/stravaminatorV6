/* This is a template file for board configuration created by MCUXpresso Project Generator. Enjoy! */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* The board name */
#define BOARD_NAME "FRDM-K64F"

#define EMPTY_MACRO                    do {} while (0)


/* The SDHC instance/channel used for board */
#define BOARD_SDHC_CD_GPIO_IRQ_HANDLER PORTB_IRQHandler

/* SDHC base address, clock and card detection pin */
#define BOARD_SDHC_BASEADDR SDHC
#define BOARD_SDHC_CLKSRC kCLOCK_CoreSysClk
#define BOARD_SDHC_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define BOARD_SDHC_IRQ SDHC_IRQn
#define BOARD_SDHC_CD_GPIO_BASE GPIOE
#define BOARD_SDHC_CD_GPIO_PIN 6U
#define BOARD_SDHC_CD_PORT_BASE PORTE
#define BOARD_SDHC_CD_PORT_IRQ PORTE_IRQn
#define BOARD_SDHC_CD_PORT_IRQ_HANDLER PORTE_IRQHandler
#define BOARD_SDHC_CD_LOGIC_RISING

#define BOARD_ACCEL_I2C_BASEADDR I2C0

/* Board accelerometer driver */
#define BOARD_ACCEL_FXOS

/* Board led color mapping */
#define LOGIC_LED_ON 0U
#define LOGIC_LED_OFF 1U
#define BOARD_LED_RED_GPIO GPIOB
#define BOARD_LED_RED_GPIO_PORT PORTB
#define BOARD_LED_RED_GPIO_PIN 22U
#define BOARD_LED_GREEN_GPIO GPIOE
#define BOARD_LED_GREEN_GPIO_PORT PORTE
#define BOARD_LED_GREEN_GPIO_PIN 26U
#define BOARD_LED_BLUE_GPIO GPIOB
#define BOARD_LED_BLUE_GPIO_PORT PORTB
#define BOARD_LED_BLUE_GPIO_PIN 21U

#ifdef DEBUG_CONFIG
#define ON_BOARD_LED
#endif

#ifndef ON_BOARD_LED

#define LED_RED_INIT(output)                EMPTY_MACRO
#define LED_RED_ON()                        EMPTY_MACRO
#define LED_RED_OFF()                       EMPTY_MACRO
#define LED_RED_TOGGLE()                    EMPTY_MACRO

#define LED_GREEN_INIT(output)              EMPTY_MACRO
#define LED_GREEN_ON()                      EMPTY_MACRO
#define LED_GREEN_OFF()                     EMPTY_MACRO
#define LED_GREEN_TOGGLE()                  EMPTY_MACRO

#define LED_BLUE_INIT(output)               EMPTY_MACRO
#define LED_BLUE_ON()                       EMPTY_MACRO
#define LED_BLUE_OFF()                      EMPTY_MACRO
#define LED_BLUE_TOGGLE()                   EMPTY_MACRO

#else

#define LED_RED_INIT(output)                                                 \
    GPIO_WritePinOutput(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, output); \
    BOARD_LED_RED_GPIO->PDDR |= (1U << BOARD_LED_RED_GPIO_PIN) /*!< Enable target LED_RED */
#define LED_RED_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn on target LED_RED */
#define LED_RED_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn off target LED_RED */
#define LED_RED_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Toggle on target LED_RED */

#define LED_GREEN_INIT(output)                                                   \
    GPIO_WritePinOutput(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, output); \
    BOARD_LED_GREEN_GPIO->PDDR |= (1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Enable target LED_GREEN */
#define LED_GREEN_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn on target LED_GREEN */
#define LED_GREEN_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn off target LED_GREEN */
#define LED_GREEN_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Toggle on target LED_GREEN */

#define LED_BLUE_INIT(output)                                                  \
    GPIO_WritePinOutput(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, output); \
    BOARD_LED_BLUE_GPIO->PDDR |= (1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Enable target LED_BLUE */
#define LED_BLUE_ON() \
    GPIO_ClearPinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn on target LED_BLUE */
#define LED_BLUE_OFF() \
    GPIO_SetPinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn off target LED_BLUE */
#define LED_BLUE_TOGGLE() \
    GPIO_TogglePinsOutput(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Toggle on target LED_BLUE */


#endif

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief initialize debug console to enable printf for this demo/example
 */
void BOARD_InitDebugConsole(void);

void BOARD_InitHardware(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
