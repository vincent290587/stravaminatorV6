#ifndef _CLOCK_CONFIG_H_
#define _CLOCK_CONFIG_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes default configuration of clocks.
 *
 */
void BOARD_InitBootClocks(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ********************** Configuration BOARD_BootClockRUN ***********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_BootClockRUN configuration
 ******************************************************************************/
#define BOARD_BOOTCLOCKRUN_CORE_CLOCK              96000000U  /*!< Core clock frequency: 96000000Hz */

/*! @brief MCG set for BOARD_BootClockRUN configuration.
 */
extern const mcg_config_t mcgConfig_BOARD_BootClockRUN;
/*! @brief SIM module set for BOARD_BootClockRUN configuration.
 */
extern const sim_clock_config_t simConfig_BOARD_BootClockRUN;
/*! @brief OSC set for BOARD_BootClockRUN configuration.
 */
extern const osc_config_t oscConfig_BOARD_BootClockRUN;

/*******************************************************************************
 * API for BOARD_BootClockRUN configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_BootClockRUN(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ********************** Configuration ClocksConfig_VLPR ************************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for ClocksConfig_VLPR configuration
 ******************************************************************************/
#define CLOCKSCONFIG_VLPR_CORE_CLOCK                4000000U  /*!< Core clock frequency: 4000000Hz */

/*! @brief MCG set for ClocksConfig_VLPR configuration.
 */
extern const mcg_config_t mcgConfig_ClocksConfig_VLPR;
/*! @brief SIM module set for ClocksConfig_VLPR configuration.
 */
extern const sim_clock_config_t simConfig_ClocksConfig_VLPR;
/*! @brief OSC set for ClocksConfig_VLPR configuration.
 */
extern const osc_config_t oscConfig_ClocksConfig_VLPR;

/*******************************************************************************
 * API for ClocksConfig_VLPR configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void ClocksConfig_VLPR(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _CLOCK_CONFIG_H_ */

