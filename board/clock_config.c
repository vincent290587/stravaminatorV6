/*
 * How to setup clock using clock driver functions:
 *
 * 1. CLOCK_SetSimSafeDivs, to make sure core clock, bus clock, flexbus clock
 *    and flash clock are in allowed range during clock mode switch.
 *
 * 2. Call CLOCK_Osc0Init to setup OSC clock, if it is used in target mode.
 *
 * 3. Set MCG configuration, MCG includes three parts: FLL clock, PLL clock and
 *    internal reference clock(MCGIRCLK). Follow the steps to setup:
 *
 *    1). Call CLOCK_BootToXxxMode to set MCG to target mode.
 *
 *    2). If target mode is FBI/BLPI/PBI mode, the MCGIRCLK has been configured
 *        correctly. For other modes, need to call CLOCK_SetInternalRefClkConfig
 *        explicitly to setup MCGIRCLK.
 *
 *    3). Don't need to configure FLL explicitly, because if target mode is FLL
 *        mode, then FLL has been configured by the function CLOCK_BootToXxxMode,
 *        if the target mode is not FLL mode, the FLL is disabled.
 *
 *    4). If target mode is PEE/PBE/PEI/PBI mode, then the related PLL has been
 *        setup by CLOCK_BootToXxxMode. In FBE/FBI/FEE/FBE mode, the PLL could
 *        be enabled independently, call CLOCK_EnablePll0 explicitly in this case.
 *
 * 4. Call CLOCK_SetSimConfig to set the clock configuration in SIM.
 */

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Clocks v3.0
processor: MK64FN1M0xxx12
package_id: MK64FN1M0VLL12
mcu_data: ksdk2_0
processor_version: 2.0.0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

#include "system_MK64F12.h"
#include "fsl_smc.h"
#include "clock_config.h"
#include "segger_wrapper.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MCG_IRCLK_DISABLE                                 0U  /*!< MCGIRCLK disabled */
#define MCG_PLL_DISABLE                                   0U  /*!< MCGPLLCLK disabled */
#define OSC_CAP0P                                         0U  /*!< Oscillator 0pF capacitor load */
#define OSC_ER_CLK_DISABLE                                0U  /*!< Disable external reference clock */
#define SIM_OSC32KSEL_OSC32KCLK_CLK                       0U  /*!< OSC32KSEL select: OSC32KCLK clock */
#define SIM_PLLFLLSEL_IRC48MCLK_CLK                       3U  /*!< PLLFLL select: IRC48MCLK clock */
#define SIM_PLLFLLSEL_MCGFLLCLK_CLK                       0U  /*!< PLLFLL select: MCGFLLCLK clock */
#define SIM_SDHC_CLK_SEL_CORE_SYSTEM_CLK                  0U  /*!< SDHC clock select: Core/system clock */
#define SIM_SDHC_CLK_SEL_PLLFLLSEL_CLK                    1U  /*!< SDHC clock select: PLLFLLSEL output clock */
#define SIM_USB_CLK_48000000HZ                     48000000U  /*!< Input SIM frequency for USB: 48000000Hz */

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* System clock frequency. */
extern uint32_t SystemCoreClock;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_CONFIG_EnableIrc48MOsc
 * Description   : This function is used to enabling IRC48M oscillator for K60_1M
 * as workaround because there is not enabled the oscillator automatically.
 *
 *END**************************************************************************/
static void CLOCK_CONFIG_EnableIrc48MOsc()
{
  /* USB clock gate enable */
  CLOCK_EnableClock(kCLOCK_Usbfs0);
  /* IRC48M oscillator enable */
  USB0->CLK_RECOVER_IRC_EN = USB_CLK_RECOVER_IRC_EN_IRC_EN_MASK | USB_CLK_RECOVER_IRC_EN_REG_EN_MASK;
  /* USB clock gate disable */
  CLOCK_DisableClock(kCLOCK_Usbfs0);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_CONFIG_SetFllExtRefDiv
 * Description   : Configure FLL external reference divider (FRDIV).
 * Param frdiv   : The value to set FRDIV.
 *
 *END**************************************************************************/
static void CLOCK_CONFIG_SetFllExtRefDiv(uint8_t frdiv)
{
    MCG->C1 = ((MCG->C1 & ~MCG_C1_FRDIV_MASK) | MCG_C1_FRDIV(frdiv));
}

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/
void BOARD_InitBootClocks(void)
{
	ClocksConfig_RUN();
}

/*******************************************************************************
 ********************** Configuration BOARD_BootClockRUN ***********************
 ******************************************************************************/
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!Configuration
name: BOARD_BootClockRUN
called_from_default_init: true
outputs:
- {id: Bus_clock.outFreq, value: 48 MHz}
- {id: Core_clock.outFreq, value: 96 MHz}
- {id: Flash_clock.outFreq, value: 24 MHz}
- {id: FlexBus_clock.outFreq, value: 32 MHz}
- {id: IRC48MCLK.outFreq, value: 48 MHz}
- {id: LPO_clock.outFreq, value: 1 kHz}
- {id: PLLFLLCLK.outFreq, value: 48 MHz}
- {id: SDHCCLK.outFreq, value: 48 MHz}
- {id: System_clock.outFreq, value: 96 MHz}
- {id: USB48MCLK.outFreq, value: 48 MHz}
settings:
- {id: MCGMode, value: PEE}
- {id: MCG.FCRDIV.scale, value: '2', locked: true}
- {id: MCG.FRDIV.scale, value: '512'}
- {id: MCG.IRCS.sel, value: MCG.FCRDIV}
- {id: MCG.IREFS.sel, value: MCG.FRDIV}
- {id: MCG.OSCSEL.sel, value: SIM.IRC48MCLK}
- {id: MCG.PLLS.sel, value: MCG.PLL}
- {id: MCG.PRDIV.scale, value: '12', locked: true}
- {id: MCG.VDIV.scale, value: '24', locked: true}
- {id: MCG_C2_OSC_MODE_CFG, value: ModeOscLowPower}
- {id: MCG_C2_RANGE0_CFG, value: Very_high}
- {id: MCG_C2_RANGE0_FRDIV_CFG, value: Very_high}
- {id: MCRFFCLKAllowConfig, value: 'no'}
- {id: OSC_CR_SYS_OSC_CAP_LOAD_CFG, value: SC18PF}
- {id: SDHCClkConfig, value: 'yes'}
- {id: SIM.OUTDIV1.scale, value: '1', locked: true}
- {id: SIM.OUTDIV2.scale, value: '2', locked: true}
- {id: SIM.OUTDIV3.scale, value: '3'}
- {id: SIM.OUTDIV4.scale, value: '4', locked: true}
- {id: SIM.PLLFLLSEL.sel, value: IRC48M.IRC48MCLK}
- {id: SIM.SDHCSRCSEL.sel, value: SIM.PLLFLLSEL}
- {id: SIM.USBSRCSEL.sel, value: SIM.USBDIV}
- {id: USBClkConfig, value: 'yes'}
sources:
- {id: IRC48M.IRC48M.outFreq, value: 48 MHz}
- {id: OSC.OSC.outFreq, value: 16 MHz}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/*******************************************************************************
 * Variables for BOARD_BootClockRUN configuration
 ******************************************************************************/
const mcg_config_t mcgConfig_BOARD_BootClockRUN =
    {
        .mcgMode = kMCG_ModePEE,                  /* PEE - PLL Engaged External */
        .irclkEnableMode = MCG_IRCLK_DISABLE,     /* MCGIRCLK disabled */
        .ircs = kMCG_IrcFast,                     /* Fast internal reference clock selected */
        .fcrdiv = 0x1U,                           /* Fast IRC divider: divided by 2 */
        .frdiv = 0x4U,                            /* FLL reference clock divider: divided by 512 */
        .drs = kMCG_DrsLow,                       /* Low frequency range */
        .dmx32 = kMCG_Dmx32Default,               /* DCO has a default range of 25% */
        .oscsel = kMCG_OscselIrc,                 /* Selects 48 MHz IRC Oscillator */
        .pll0Config =
            {
                .enableMode = MCG_PLL_DISABLE,    /* MCGPLLCLK disabled */
                .prdiv = 0xbU,                    /* PLL Reference divider: divided by 12 */
                .vdiv = 0x0U,                     /* VCO divider: multiplied by 24 */
            },
    };
const sim_clock_config_t simConfig_BOARD_BootClockRUN =
    {
        .pllFllSel = SIM_PLLFLLSEL_IRC48MCLK_CLK, /* PLLFLL select: IRC48MCLK clock */
        .er32kSrc = SIM_OSC32KSEL_OSC32KCLK_CLK,  /* OSC32KSEL select: OSC32KCLK clock */
        .clkdiv1 = 0x1230000U,                    /* SIM_CLKDIV1 - OUTDIV1: /1, OUTDIV2: /2, OUTDIV3: /3, OUTDIV4: /4 */
    };
const osc_config_t oscConfig_BOARD_BootClockRUN =
    {
        .freq = 0U,                               /* Oscillator frequency: 0Hz */
        .capLoad = (kOSC_Cap2P | kOSC_Cap16P),    /* Oscillator capacity load: 18pF */
        .workMode = kOSC_ModeOscLowPower,         /* Oscillator low power */
        .oscerConfig =
            {
                .enableMode = OSC_ER_CLK_DISABLE, /* Disable external reference clock */
            }
    };

/*******************************************************************************
 * Code for BOARD_BootClockRUN configuration
 ******************************************************************************/
void ClocksConfig_RUN(void)
{
    /* Set the system clock dividers in SIM to safe value. */
    CLOCK_SetSimSafeDivs();
    /* Enable IRC48M oscillator for K60_1M as workaround because there is not enabled the oscillator automatically. */
    CLOCK_CONFIG_EnableIrc48MOsc();
    /* Configure FLL external reference divider (FRDIV). */
    CLOCK_CONFIG_SetFllExtRefDiv(mcgConfig_BOARD_BootClockRUN.frdiv);
    /* Set MCG to PEE mode. */
    CLOCK_BootToPeeMode(mcgConfig_BOARD_BootClockRUN.oscsel,
                        kMCG_PllClkSelPll0,
                        &mcgConfig_BOARD_BootClockRUN.pll0Config);
    /* Set the clock configuration in SIM module. */
    CLOCK_SetSimConfig(&simConfig_BOARD_BootClockRUN);
    /* Set SystemCoreClock variable. */
    SystemCoreClock = BOARD_BOOTCLOCKRUN_CORE_CLOCK;
    // update segger
    segger_update_clocks();
    /* Enable USB FS clock. */
    CLOCK_EnableUsbfs0Clock(kCLOCK_UsbSrcIrc48M, SIM_USB_CLK_48000000HZ);
    /* Set SDHC clock source. */
    CLOCK_SetSdhc0Clock(SIM_SDHC_CLK_SEL_PLLFLLSEL_CLK);

}

/*******************************************************************************
 ********************** Configuration ClocksConfig_VLPR ************************
 ******************************************************************************/
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!Configuration
name: ClocksConfig_VLPR
outputs:
- {id: Bus_clock.outFreq, value: 4 MHz}
- {id: Core_clock.outFreq, value: 4 MHz}
- {id: Flash_clock.outFreq, value: 800 kHz}
- {id: FlexBus_clock.outFreq, value: 2 MHz}
- {id: LPO_clock.outFreq, value: 1 kHz}
- {id: SDHCCLK.outFreq, value: 4 MHz}
- {id: System_clock.outFreq, value: 4 MHz}
settings:
- {id: MCGMode, value: BLPI}
- {id: powerMode, value: VLPR}
- {id: MCG.CLKS.sel, value: MCG.IRCS}
- {id: MCG.FCRDIV.scale, value: '1', locked: true}
- {id: MCG.IRCS.sel, value: MCG.FCRDIV}
- {id: SDHCClkConfig, value: 'yes'}
- {id: SIM.OUTDIV4.scale, value: '5'}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/*******************************************************************************
 * Variables for ClocksConfig_VLPR configuration
 ******************************************************************************/
const mcg_config_t mcgConfig_ClocksConfig_VLPR =
    {
        .mcgMode = kMCG_ModeBLPI,                 /* BLPI - Bypassed Low Power Internal */
        .irclkEnableMode = MCG_IRCLK_DISABLE,     /* MCGIRCLK disabled */
        .ircs = kMCG_IrcFast,                     /* Fast internal reference clock selected */
        .fcrdiv = 0x0U,                           /* Fast IRC divider: divided by 1 */
        .frdiv = 0x0U,                            /* FLL reference clock divider: divided by 1 */
        .drs = kMCG_DrsLow,                       /* Low frequency range */
        .dmx32 = kMCG_Dmx32Default,               /* DCO has a default range of 25% */
        .oscsel = kMCG_OscselOsc,                 /* Selects System Oscillator (OSCCLK) */
        .pll0Config =
            {
                .enableMode = MCG_PLL_DISABLE,    /* MCGPLLCLK disabled */
                .prdiv = 0x0U,                    /* PLL Reference divider: divided by 1 */
                .vdiv = 0x0U,                     /* VCO divider: multiplied by 24 */
            },
    };
const sim_clock_config_t simConfig_ClocksConfig_VLPR =
    {
        .pllFllSel = SIM_PLLFLLSEL_MCGFLLCLK_CLK, /* PLLFLL select: MCGFLLCLK clock */
        .er32kSrc = SIM_OSC32KSEL_OSC32KCLK_CLK,  /* OSC32KSEL select: OSC32KCLK clock */
        .clkdiv1 = 0x140000U,                     /* SIM_CLKDIV1 - OUTDIV1: /1, OUTDIV2: /1, OUTDIV3: /2, OUTDIV4: /5 */
    };
const osc_config_t oscConfig_ClocksConfig_VLPR =
    {
        .freq = 0U,                               /* Oscillator frequency: 0Hz */
        .capLoad = (OSC_CAP0P),                   /* Oscillator capacity load: 0pF */
        .workMode = kOSC_ModeExt,                 /* Use external clock */
        .oscerConfig =
            {
                .enableMode = OSC_ER_CLK_DISABLE, /* Disable external reference clock */
            }
    };

/*******************************************************************************
 * Code for ClocksConfig_VLPR configuration
 ******************************************************************************/
void ClocksConfig_VLPR(void)
{
#if (USE_SVIEW==0)
    /* Set the system clock dividers in SIM to safe value. */
    CLOCK_SetSimSafeDivs();
    /* Set MCG to BLPI mode. CLOCK_BootToBlpiMode CLOCK_SetInternalRefClkConfig */
    CLOCK_SetInternalRefClkConfig(mcgConfig_ClocksConfig_VLPR.fcrdiv,
    		mcgConfig_ClocksConfig_VLPR.ircs,
			mcgConfig_ClocksConfig_VLPR.irclkEnableMode);

    /* Enter FBE. */
    CLOCK_ExternalModeToFbeModeQuick();
    /* Enter FBI. */
    CLOCK_SetFbiMode(mcgConfig_ClocksConfig_VLPR.dmx32,
    		mcgConfig_ClocksConfig_VLPR.drs,
			NULL);
    /* Enter BLPI. */
    CLOCK_SetLowPowerEnable(true);

    /* Set the clock configuration in SIM module. */
    CLOCK_SetSimConfig(&simConfig_ClocksConfig_VLPR);

    /* Set VLPR power mode. */
    SMC_SetPowerModeProtection(SMC, kSMC_AllowPowerModeAll);
#if (defined(FSL_FEATURE_SMC_HAS_LPWUI) && FSL_FEATURE_SMC_HAS_LPWUI)
    SMC_SetPowerModeVlpr(SMC, false);
#else
    SMC_SetPowerModeVlpr(SMC);
#endif
    while (SMC_GetPowerModeState(SMC) != kSMC_PowerStateVlpr)
    {
    }
    /* Set SystemCoreClock variable. */
    SystemCoreClock = CLOCKSCONFIG_VLPR_CORE_CLOCK;
    // update segger
    segger_update_clocks();
    /* Set SDHC clock source. */
    CLOCK_SetSdhc0Clock(SIM_SDHC_CLK_SEL_CORE_SYSTEM_CLK);
#endif
}

