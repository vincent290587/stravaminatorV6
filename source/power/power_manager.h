/*
 * power_manager.h
 *
 *  Created on: 15 oct. 2017
 *      Author: Vincent
 */

#ifndef SOURCE_POWER_POWER_MANAGER_H_
#define SOURCE_POWER_POWER_MANAGER_H_


#include "fsl_common.h"
#include "fsl_smc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Power mode definition used in application. */
typedef enum _app_power_mode
{
    kAPP_PowerModeMin = 200,
    kAPP_PowerModeRun120,   /* Normal RUN mode 120MHz */
    kAPP_PowerModeRun24,   /* Normal RUN mode 24MHz */
    kAPP_PowerModeWait,  /* WAIT mode. */
    kAPP_PowerModeStop,  /* STOP mode. */
    kAPP_PowerModeVlpr,  /* VLPR mode. */
    kAPP_PowerModeVlpw,  /* VLPW mode. */
    kAPP_PowerModeVlps,  /* VLPS mode. */
    kAPP_PowerModeLls,   /* LLS mode. */
    kAPP_PowerModeVlls0, /* VLLS0 mode. */
    kAPP_PowerModeVlls1, /* VLLS1 mode. */
    kAPP_PowerModeVlls2, /* VLLS2 mode. */
    kAPP_PowerModeVlls3, /* VLLS3 mode. */
    kAPP_PowerModeMax
} app_power_mode_t;

typedef enum _app_wakeup_source
{
    kAPP_WakeupSourceLptmr, /*!< Wakeup by LPTMR.        */
    kAPP_WakeupSourcePin    /*!< Wakeup by external pin. */
} app_wakeup_source_t;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

int power_manager_init(void);

int power_manager_run(app_power_mode_t targetPowerMode);

void APP_ShowPowerMode(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* SOURCE_POWER_POWER_MANAGER_H_ */
