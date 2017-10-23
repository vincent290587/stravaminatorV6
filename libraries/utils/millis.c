/*
 * millis.c
 *
 *  Created on: 5 oct. 2017
 *      Author: Vincent
 */


#include <stdint.h>
#include "clock_config.h"
#include "fsl_lptmr.h"
#include "segger_wrapper.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Get source clock for LPTMR driver */
#define LPTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_LpoClk)
/* Define LPTMR microseconds counts value */
#define LPTMR_USEC_COUNT 1000U

/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile uint32_t lptmrCounter = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/
void LPTMR0_IRQHandler(void)
{
    LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
    lptmrCounter++;
    /*
     * Workaround for TWR-KV58: because write buffer is enabled, adding
     * memory barrier instructions to make sure clearing interrupt flag completed
     * before go out ISR
     */
    __DSB();
    __ISB();
}

/**
 *
 */
void millis_init(void) {

	lptmr_config_t lptmrConfig;

	/* Configure LPTMR */
	/*
	 * lptmrConfig.timerMode = kLPTMR_TimerModeTimeCounter;
	 * lptmrConfig.pinSelect = kLPTMR_PinSelectInput_0;
	 * lptmrConfig.pinPolarity = kLPTMR_PinPolarityActiveHigh;
	 * lptmrConfig.enableFreeRunning = false;
	 * lptmrConfig.bypassPrescaler = true;
	 * lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
	 * lptmrConfig.value = kLPTMR_Prescale_Glitch_0;
	 */
	LPTMR_GetDefaultConfig(&lptmrConfig);

	/* Initialize the LPTMR */
	LPTMR_Init(LPTMR0, &lptmrConfig);

	/*
	 * Set timer period.
	 * Note : the parameter "ticks" of LPTMR_SetTimerPeriod should be equal or greater than 1.
	 */
	LPTMR_SetTimerPeriod(LPTMR0, USEC_TO_COUNT(LPTMR_USEC_COUNT, LPTMR_SOURCE_CLOCK));

	/* Enable timer interrupt */
	LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);

	/* Enable at the NVIC */
	EnableIRQ(LPTMR0_IRQn);

	lptmrCounter = 0;

	/* Start counting */
	LPTMR_StartTimer(LPTMR0);

}

/** @brief Returns the current time
 *
 * @return The internal time
 */
uint32_t millis(void) {
	return lptmrCounter;
}

/** @brief Delays execution by sleeping
 *
 * @param delay_ The delay in ms
 */
void delay_ms(uint32_t delay_) {
	uint32_t start = lptmrCounter;

	W_SYSVIEW_OnIdle();

	while (lptmrCounter < start + delay_ && lptmrCounter >= start) {
		//__asm("NOP"); /* delay */
		__asm("WFI"); /* sleep */
	}
}

/**
 *
 */
void sleep(void) {
	__asm("WFI"); /* sleep */
}
