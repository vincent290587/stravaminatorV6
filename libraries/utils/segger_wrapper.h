/*
 * segger_wrapper.h
 *
 *  Created on: 5 oct. 2017
 *      Author: Vincent
 */

#ifndef LIBRARIES_UTILS_SEGGER_WRAPPER_H_
#define LIBRARIES_UTILS_SEGGER_WRAPPER_H_


#include "MK64F12.h"
#include <stddef.h>
#include "fsl_common.h"
#include "SEGGER_SYSVIEW.h"
#include "SEGGER_RTT.h"

/////////    PARAMETERS

#define USE_RTT           0
#define USE_SVIEW         1

#define RTT_LOG_CHANNEL   0
#define RTT_GRAPH_CHANNEL 1

/////////    DEFINES

#define EMPTY_MACRO                    do {} while (0)

#if USE_RTT
#define LOG_INFO(...)                  SEGGER_RTT_printf(RTT_LOG_CHANNEL, __VA_ARGS__)
#define LOG_FLUSH(X)                   SEGGER_RTT_WriteBlock(RTT_LOG_CHANNEL," ",0)
#define LOG_SET_TERM(X)                SEGGER_RTT_SetTerminal(X)
#else
#define LOG_INFO(...)                  EMPTY_MACRO
#define LOG_FLUSH(...)                 EMPTY_MACRO
#define LOG_SET_TERM(X)                EMPTY_MACRO
#endif

#if USE_SVIEW
#define W_SYSVIEW_OnIdle(...)          SEGGER_SYSVIEW_OnIdle()
#define W_SYSVIEW_OnTaskStartExec(X)   SEGGER_SYSVIEW_OnTaskStartExec(X)
#define W_SYSVIEW_OnTaskStopExec(X)    {                                        \
										SEGGER_SYSVIEW_OnTaskStopReady(X,0);    \
										SEGGER_SYSVIEW_OnIdle();                \
										}
#define W_SYSVIEW_OnTaskCreate(X)      SEGGER_SYSVIEW_OnTaskCreate(X)
#else
#define W_SYSVIEW_OnIdle(...)          EMPTY_MACRO
#define W_SYSVIEW_OnTaskStartExec(X)   EMPTY_MACRO
#define W_SYSVIEW_OnTaskStopExec(X)    EMPTY_MACRO
#define W_SYSVIEW_OnTaskCreate(X)      EMPTY_MACRO
#endif

#define SYSVIEW_MAX_NOF_TASKS          5

#define TASK_BASE                      (36u)

#define SPI_TASK                       (TASK_BASE + 1u)
#define LCD_TASK                       (TASK_BASE + 2u)

#define SD_ACCESS_TASK                 (TASK_BASE + 3u)

#define SEG_PERF_TASK                  (TASK_BASE + 4u)


/////////    FUNCTIONS

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

void segger_init(void);

void segger_update_clocks(void);

void segger_send(UART_Type *base, const uint8_t *buffer, size_t length);

status_t segger_recv(UART_Type *base, uint8_t *buffer, size_t length);

#if defined(__cplusplus)
}
#endif /* _cplusplus */

#endif /* LIBRARIES_UTILS_SEGGER_WRAPPER_H_ */
