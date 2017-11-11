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
 ********************** Configuration ClockConfig_HSRUN ************************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for ClockConfig_HSRUN configuration
 ******************************************************************************/
#define CLOCKCONFIG_HSRUN_CORE_CLOCK              120000000U  /*!< Core clock frequency: 120000000Hz */

/*! @brief MCG set for ClockConfig_HSRUN configuration.
 */
extern const mcg_config_t mcgConfig_ClockConfig_HSRUN;
/*! @brief SIM module set for ClockConfig_HSRUN configuration.
 */
extern const sim_clock_config_t simConfig_ClockConfig_HSRUN;
/*! @brief OSC set for ClockConfig_HSRUN configuration.
 */
extern const osc_config_t oscConfig_ClockConfig_HSRUN;

/*******************************************************************************
 * API for ClockConfig_HSRUN configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void ClockConfig_HSRUN(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ********************** Configuration ClocksConfig_LPRUN ***********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for ClocksConfig_LPRUN configuration
 ******************************************************************************/
#define CLOCKSCONFIG_LPRUN_CORE_CLOCK              24000000U  /*!< Core clock frequency: 24000000Hz */

/*! @brief MCG set for ClocksConfig_LPRUN configuration.
 */
extern const mcg_config_t mcgConfig_ClocksConfig_LPRUN;
/*! @brief SIM module set for ClocksConfig_LPRUN configuration.
 */
extern const sim_clock_config_t simConfig_ClocksConfig_LPRUN;
/*! @brief OSC set for ClocksConfig_LPRUN configuration.
 */
extern const osc_config_t oscConfig_ClocksConfig_LPRUN;

/*******************************************************************************
 * API for ClocksConfig_LPRUN configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void ClocksConfig_LPRUN(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ********************** Configuration ClocksConfig_VLPR ************************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for ClocksConfig_VLPR configuration
 ******************************************************************************/
#define CLOCKSCONFIG_VLPR_CORE_CLOCK                2000000U  /*!< Core clock frequency: 2000000Hz */

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

void ClocksConfig_VLPR(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _CLOCK_CONFIG_H_ */

