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
#include "uart0.h"
#include "uart2.h"

#include "pin_mux.h"
#include "fsl_pmc.h"
#include "fsl_uart.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_DEBUG_UART_BAUDRATE 9600                 /* Debug console baud rate.           */
#define APP_DEBUG_UART_CLKSRC_NAME kCLOCK_CoreSysClk /* System clock.       */

#define APP_WAKEUP_BUTTON_GPIO BOARD_SW2_GPIO
#define APP_WAKEUP_BUTTON_PORT BOARD_SW2_PORT
#define APP_WAKEUP_BUTTON_GPIO_PIN BOARD_SW2_GPIO_PIN
#define APP_WAKEUP_BUTTON_IRQ BOARD_SW2_IRQ
#define APP_WAKEUP_BUTTON_IRQ_HANDLER BOARD_SW2_IRQ_HANDLER
#define APP_WAKEUP_BUTTON_NAME BOARD_SW2_NAME
#define APP_WAKEUP_BUTTON_IRQ_TYPE kPORT_InterruptFallingEdge

/* Debug console RX pin: PORTB16 MUX: 3 */
#define DEBUG_CONSOLE_RX_PORT PORTB
#define DEBUG_CONSOLE_RX_GPIO GPIOB
#define DEBUG_CONSOLE_RX_PIN 16
#define DEBUG_CONSOLE_RX_PINMUX kPORT_MuxAlt3
/* Debug console TX pin: PORTB17 MUX: 3 */
#define DEBUG_CONSOLE_TX_PORT PORTB
#define DEBUG_CONSOLE_TX_GPIO GPIOB
#define DEBUG_CONSOLE_TX_PIN 17
#define DEBUG_CONSOLE_TX_PINMUX kPORT_MuxAlt3
#define CORE_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void APP_PowerPreSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode);
void APP_PowerPostSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode);

/*
 * Set the clock configuration for RUN mode from VLPR mode.
 */
extern void APP_SetClockRunFromVlpr(void);

/*
 * Set the clock configuration for VLPR mode.
 */
//extern void APP_SetClockVlpr(void);

/*
 * Hook function called before power mode switch.
 */
extern void APP_PowerPreSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode);

/*
 * Hook function called after power mode switch.
 */
extern void APP_PowerPostSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static smc_power_state_t curPowerState;
/*******************************************************************************
 * Code
 ******************************************************************************/


void APP_PowerPreSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode)
{
//	if ((kSMC_PowerStateVlpr == originPowerState) && (kSMC_PowerStateRun == SMC_GetPowerModeState(SMC)))
//	{
//
//	}

	uart0_uninit();
	uart2_uninit();

}

void APP_PowerPostSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode)
{
	if ((kAPP_PowerModeRun != targetMode) && (kAPP_PowerModeVlpr != targetMode))
	{
		/*
		 * Debug console RX pin is set to disable for current leakage, nee to re-configure pinmux.
		 * Debug console TX pin: Don't need to change.
		 */
//		PORT_SetPinMux(DEBUG_CONSOLE_RX_PORT, DEBUG_CONSOLE_RX_PIN, DEBUG_CONSOLE_RX_PINMUX);
	}


//	if ((kSMC_PowerStateVlpr == originPowerState) && (kSMC_PowerStateRun == SMC_GetPowerModeState(SMC)))
//	{
//		CLOCK_EnableUsbfs0Clock(USB_FS_CLK_SRC, USB_FS_CLK_FREQ);
//
//	}

	// Initialize the UART.
	uart_config_t uartConfig;
	UART_GetDefaultConfig(&uartConfig);
	uartConfig.enableTx = true;
	uartConfig.enableRx = true;
	uartConfig.baudRate_Bps = 9600U;
	uart0_init(&uartConfig);

	uartConfig.baudRate_Bps = 115200U;
	uart2_init(&uartConfig);

	/*
	 * If enter stop modes when MCG in PEE mode, then after wakeup, the MCG is in PBE mode,
	 * need to enter PEE mode manually.
	 */
//	if ((kAPP_PowerModeRun != targetMode) && (kAPP_PowerModeWait != targetMode) && (kAPP_PowerModeVlpw != targetMode) &&
//			(kAPP_PowerModeVlpr != targetMode))
//	{
//		if (kSMC_PowerStateRun == originPowerState)
//		{
//			/* Wait for PLL lock. */
//			while (!(kMCG_Pll0LockFlag & CLOCK_GetStatusFlags()))
//			{
//			}
//			CLOCK_SetPeeMode();
//		}
//	}

//	APP_InitDebugConsole();
}

//void APP_WAKEUP_BUTTON_IRQ_HANDLER(void)
//{
//	if ((1U << APP_WAKEUP_BUTTON_GPIO_PIN) & PORT_GetPinsInterruptFlags(APP_WAKEUP_BUTTON_PORT))
//	{
//		/* Disable interrupt. */
//		PORT_SetPinInterruptConfig(APP_WAKEUP_BUTTON_PORT, APP_WAKEUP_BUTTON_GPIO_PIN, kPORT_InterruptOrDMADisabled);
//		PORT_ClearPinsInterruptFlags(APP_WAKEUP_BUTTON_PORT, (1U << APP_WAKEUP_BUTTON_GPIO_PIN));
//	}
//}

void APP_ShowPowerMode(void)
{
	switch (SMC_GetPowerModeState(SMC))
	{
	case kSMC_PowerStateRun:
		LOG_INFO("    Power mode: RUN\r\n");
		break;
	case kSMC_PowerStateVlpr:
		LOG_INFO("    Power mode: VLPR\r\n");
		break;
	default:
		LOG_INFO("    Power mode wrong\r\n");
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
	if (((kAPP_PowerModeRun == targetPowerMode) && (kSMC_PowerStateRun == curPowerState)) ||
			((kAPP_PowerModeVlpr == targetPowerMode) && (kSMC_PowerStateVlpr == curPowerState)))
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
		ClocksConfig_VLPR();
//		APP_SetClockVlpr();
//		SMC_SetPowerModeVlpr(SMC, false);
//		while (kSMC_PowerStateVlpr != SMC_GetPowerModeState(SMC))
//		{
//		}
		break;

	case kAPP_PowerModeRun:
		/* Power mode change. */

		SMC_SetPowerModeRun(SMC);
		while (kSMC_PowerStateRun != SMC_GetPowerModeState(SMC))
		{
		}

		/* If enter RUN from VLPR, change clock after the power mode change. */
		if (kSMC_PowerStateVlpr == curPowerState)
		{
			ClocksConfig_RUN();
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

	if (kRCM_SourceWakeup & RCM_GetPreviousResetSources(RCM)) /* Wakeup from VLLS. */
	{
		LOG_INFO("\r\nMCU wakeup from VLLS modes...\r\n");
	}

	curPowerState = SMC_GetPowerModeState(SMC);

	LOG_INFO("\r\n####################  Power Mode Switch  ####################\n\r\n");
	LOG_INFO("    Core Clock = %dHz \r\n", CLOCK_GetFreq(kCLOCK_CoreSysClk));

	APP_ShowPowerMode();

	/* If could not set the target power mode, loop continue. */
	if (!APP_CheckPowerMode(targetPowerMode))
	{
		LOG_INFO("\r\n\r\nMode not set\r\n\r\n");
		return 1;
	}

	/* If target mode is RUN/VLPR/HSRUN, don't need to set wakeup source. */

	APP_PowerPreSwitchHook(curPowerState, targetPowerMode);

	APP_PowerModeSwitch(targetPowerMode);

	APP_PowerPostSwitchHook(curPowerState, targetPowerMode);

	APP_ShowPowerMode();

	return 0;
}



