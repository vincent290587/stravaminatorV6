/*
 * segger_wrapper.h
 *
 *  Created on: 5 oct. 2017
 *      Author: Vincent
 */

#ifndef LIBRARIES_UTILS_SEGGER_WRAPPER_H_
#define LIBRARIES_UTILS_SEGGER_WRAPPER_H_

#include "SEGGER_SYSVIEW.h"
#include "SEGGER_RTT.h"


/////////    PARAMETERS

#define USE_RTT           1
#define USE_SVIEW         0

#define RTT_LOG_CHANNEL   0


/////////    DEFINES

#define EMPTY_MACRO                    do {} while (0)

#if USE_RTT
#define LOG_INFO(...)                  SEGGER_RTT_printf(RTT_LOG_CHANNEL, __VA_ARGS__)
#else
#define LOG_INFO(...)                  EMPTY_MACRO
#endif

#if USE_SVIEW
#define W_SYSVIEW_OnIdle(...)          SEGGER_SYSVIEW_OnIdle()
#define W_SYSVIEW_OnTaskStartExec(X)   SEGGER_SYSVIEW_OnTaskStartExec(X)
#define W_SYSVIEW_OnTaskStopExec(X)    SEGGER_SYSVIEW_OnTaskTerminate(X)
#else
#define W_SYSVIEW_OnIdle(...)          EMPTY_MACRO
#define W_SYSVIEW_OnTaskStartExec(X)   EMPTY_MACRO
#define W_SYSVIEW_OnTaskStopExec(X)    EMPTY_MACRO
#endif


#define SPI_TASK                       1u
#define LCD_BUF_TASK                   2u

#define SD_ACCESS_TASK                 3u

#define SEG_PERF_TASK                  4u

/////////    FUNCTIONS

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

void segger_init(void);

#if defined(__cplusplus)
}
#endif /* _cplusplus */

#endif /* LIBRARIES_UTILS_SEGGER_WRAPPER_H_ */
