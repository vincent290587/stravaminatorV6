/*
 * power_manager.c
 *
 *  Created on: 15 oct. 2017
 *      Author: Vincent
 */


#include "fsl_common.h"
#include "fsl_smc.h"
#include "fsl_llwu.h"
#include "fsl_rcm.h"
#include "fsl_lptmr.h"
#include "fsl_port.h"
#include "board.h"
#include "segger_wrapper.h"
#include "power_manager.h"
#include "clock_config.h"
#include "composite.h"
#include "uart2.h"
#include "dma_spi0.h"
#include "int_i2c0.h"
#include "uart0.h"
#include "GPSMGMT.h"

#include "pin_mux.h"
#include "fsl_pmc.h"
#include "fsl_uart.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define CORE_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void APP_PowerPreSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode);
void APP_PowerPostSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static smc_power_state_t curPowerState;
static app_power_mode_t  curAppMode;

/*******************************************************************************
 * Code
 ******************************************************************************/

void sleep(void) {
	SMC_SetPowerModeWait(SMC);
}

void APP_PowerPreSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode)
{
	// if the clocks were changed, peripherals must be re-init
	if ((targetMode != kAPP_PowerModeWait &&
			curAppMode != kAPP_PowerModeWait) ||
			(targetMode != kAPP_PowerModeStop &&
					curAppMode != kAPP_PowerModeStop)) {

		gps_uart_stop();

		i2c0_uninit();

		//	dma_spi0_uninit();
		DSPI_Enable(SPI0, false);

	}
}

void APP_PowerPostSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode)
{

	// if the clocks were changed, peripherals must be re-init
	if ((targetMode != kAPP_PowerModeWait &&
			curAppMode != kAPP_PowerModeWait) ||
			(targetMode != kAPP_PowerModeStop &&
						curAppMode != kAPP_PowerModeStop)) {

	    // update segger
	    segger_update_clocks();

		// Re-init the UART.
	    gps_uart_resume();

		i2c0_init();

		// only init SPI if not going to VLP modes
		if (kAPP_PowerModeVlpr != targetMode &&
				kAPP_PowerModeVlpw != targetMode) {
//			dma_spi0_init();

			dma_spi0_update_clocks();

			DSPI_Enable(SPI0, true);
		}
	}

	curAppMode = targetMode;
}


void APP_ShowPowerMode(void)
{
	switch (SMC_GetPowerModeState(SMC))
	{
	case kSMC_PowerStateRun:
		LOG_INFO("Power mode: RUN %u MHz\r\n", CORE_CLK_FREQ / 1000000);
		break;
	case kSMC_PowerStateVlpr:
		LOG_INFO("Power mode: VLPR %u MHz\r\n", CORE_CLK_FREQ / 1000000);
		break;
	default:
		LOG_INFO("Power mode wrong\r\n");
		break;
	}
}

/*
 * Check whether could switch to target power mode from current mode.
 * Return true if could switch, return false if could not switch.
 */
bool APP_CheckPowerMode(app_power_mode_t targetPowerMode)
{
	bool modeValid = true;

	/*
	 * Check wether the mode change is allowed.
	 *
	 * 1. If current mode is HSRUN mode, the target mode must be RUN mode.
	 * 2. If current mode is RUN mode, the target mode must not be VLPW mode.
	 * 3. If current mode is VLPR mode, the target mode must not be HSRUN/WAIT/STOP mode.
	 * 4. If already in the target mode.
	 */
	switch (curPowerState)
	{
	case kSMC_PowerStateRun:
		if (kAPP_PowerModeVlpw == targetPowerMode)
		{
			LOG_INFO("Could not enter VLPW mode from RUN mode.\r\n");
			modeValid = false;
		}
		break;

	case kSMC_PowerStateVlpr:
		if ((kAPP_PowerModeWait == targetPowerMode) || (kAPP_PowerModeStop == targetPowerMode))
		{
			LOG_INFO("Could not enter HSRUN/STOP/WAIT modes from VLPR mode.\r\n");
			modeValid = false;
		}
		break;
	default:
		LOG_INFO("Wrong power state.\r\n");
		modeValid = false;
		break;
	}

	if (!modeValid)
	{
		return false;
	}

	/* Don't need to change power mode if current mode is already the target mode. */
	if (curAppMode == targetPowerMode)
	{
		LOG_INFO("Already in the target power mode.\r\n");
		return false;
	}

	return true;
}

/*
 * Power mode switch.
 */
void APP_PowerModeSwitch(app_power_mode_t targetPowerMode)
{
	switch (targetPowerMode)
	{
	case kAPP_PowerModeVlpr:

		if (kSMC_PowerStateVlpw == curPowerState)
		{
			SMC_SetPowerModeVlpr(SMC, false);
		} else {
			ClocksConfig_VLPR();
		}
		break;

	case kAPP_PowerModeRun120:

		/* Power mode change. */
		SMC_SetPowerModeRun(SMC);
		while (kSMC_PowerStateRun != SMC_GetPowerModeState(SMC))
		{
		}

		/* If enter RUN from VLPR, change clock after the power mode change. */
		if (kAPP_PowerModeWait != curAppMode ||
				SystemCoreClock != CLOCKCONFIG_HSRUN_CORE_CLOCK)
		{
			ClockConfig_HSRUN();
		}
		break;

	case kAPP_PowerModeRun24:
		/* Power mode change. */

		SMC_SetPowerModeRun(SMC);
		while (kSMC_PowerStateRun != SMC_GetPowerModeState(SMC))
		{
		}

		/* If enter RUN from VLPR, change clock after the power mode change. */
		if (kAPP_PowerModeWait != curAppMode ||
				SystemCoreClock != CLOCKSCONFIG_LPRUN_CORE_CLOCK)
		{
			ClocksConfig_LPRUN();
		}
		break;

	case kAPP_PowerModeWait:
		SMC_PreEnterWaitModes();
		SMC_SetPowerModeWait(SMC);
		SMC_PostExitWaitModes();
		break;

	case kAPP_PowerModeStop:
		SMC_PreEnterStopModes();
		SMC_SetPowerModeStop(SMC, kSMC_PartialStop);
		SMC_PostExitStopModes();
		break;

	case kAPP_PowerModeVlpw:
		SMC_PreEnterWaitModes();
		SMC_SetPowerModeVlpw(SMC);
		SMC_PostExitWaitModes();
		break;

	case kAPP_PowerModeVlps:
		SMC_PreEnterStopModes();
		SMC_SetPowerModeVlps(SMC);
		SMC_PostExitStopModes();
		break;

	default:
		LOG_INFO("Wrong value");
		break;
	}
}

/**
 *
 * @return
 */
int power_manager_init(void)
{
	/* Power related. */
	SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);

	curAppMode = kAPP_PowerModeRun120;

	if (kRCM_SourceWakeup & RCM_GetPreviousResetSources(RCM)) /* Wakeup from VLLS. */
	{
		PMC_ClearPeriphIOIsolationFlag(PMC);
		NVIC_ClearPendingIRQ(LLWU_IRQn);
	}

	return 0;
}

/**
 *
 * @param targetPowerMode
 * @return
 */
int power_manager_run(app_power_mode_t targetPowerMode) {

	curPowerState = SMC_GetPowerModeState(SMC);

	/* If could not set the target power mode, loop continue. */
	if (!APP_CheckPowerMode(targetPowerMode))
	{
		return 1;
	}

	// wait for finishing UART0 RX transmission
	while (uart0_get_last_rx_age() < 5) {
		sleep();
	}

	APP_PowerPreSwitchHook(curPowerState, targetPowerMode);

	APP_PowerModeSwitch(targetPowerMode);

	APP_PowerPostSwitchHook(curPowerState, targetPowerMode);

	APP_ShowPowerMode();

	return 0;
}


app_power_mode_t power_manager_get_mode(void) {

	return curAppMode;

}
