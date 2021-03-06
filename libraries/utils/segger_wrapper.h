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
#include "usb_parser.h"
#include "SEGGER_SYSVIEW.h"
#include "SEGGER_RTT.h"

/////////    PARAMETERS

// force RTT
//#ifdef USE_RTT
//#undef USE_RTT
//#endif

#ifndef USE_RTT
#define USE_RTT           1
#endif

#define USE_SVIEW         0
#define USE_GRAPH         0

/////////    DEFINES

#define RTT_LOG_CHANNEL   0
#define RTT_GRAPH_CHANNEL 1

#define EMPTY_MACRO                    do {} while (0)


#if USE_RTT
#define LOG_INFO(...)                  SEGGER_RTT_printf(RTT_LOG_CHANNEL, __VA_ARGS__)
#define LOG_DEBUG(...)                 EMPTY_MACRO
#define LOG_ERROR(...)                 LOG_INFO(__VA_ARGS__)
#define LOG_FLUSH(X)                   SEGGER_RTT_WriteBlock(RTT_LOG_CHANNEL," ",0)
#define LOG_SET_TERM(X)                SEGGER_RTT_SetTerminal(X)
#elif USE_SVIEW
//#define LOG_INFO(...)                  SEGGER_SYSVIEW_PrintfHost(__VA_ARGS__)
#define LOG_INFO(...)                  EMPTY_MACRO
#define LOG_DEBUG(...)                 EMPTY_MACRO
#define LOG_ERROR(...)                 SEGGER_SYSVIEW_ErrorfHost(__VA_ARGS__)
#define LOG_FLUSH(...)                 EMPTY_MACRO
#define LOG_SET_TERM(X)                EMPTY_MACRO
#else
#define LOG_INFO(...)                  usb_printf(__VA_ARGS__)
#define LOG_DEBUG(...)                 EMPTY_MACRO
#define LOG_ERROR(...)                 usb_printf(__VA_ARGS__)
#define LOG_GRAPH(...)                 EMPTY_MACRO
#define LOG_FLUSH(...)                 EMPTY_MACRO
#define LOG_SET_TERM(X)                EMPTY_MACRO
#endif

#if USE_SVIEW
//#define W_SYSVIEW_RecordEnterISR(...)  SEGGER_SYSVIEW_RecordEnterISR()
//#define W_SYSVIEW_RecordExitISR(...)   SEGGER_SYSVIEW_RecordExitISR()
#define W_SYSVIEW_RecordEnterISR(...)  EMPTY_MACRO
#define W_SYSVIEW_RecordExitISR(...)   EMPTY_MACRO
#define W_SYSVIEW_OnIdle(...)          SEGGER_SYSVIEW_OnIdle()
#define W_SYSVIEW_OnTaskStartExec(X)   SEGGER_SYSVIEW_OnTaskStartExec(X)
#define W_SYSVIEW_OnTaskStopExec(X)    SEGGER_SYSVIEW_OnTaskTerminate(X);SEGGER_SYSVIEW_OnIdle()
#define W_SYSVIEW_OnTaskCreate(X)      SEGGER_SYSVIEW_OnTaskCreate(X)
#else
#define W_SYSVIEW_RecordEnterISR(...)  EMPTY_MACRO
#define W_SYSVIEW_RecordExitISR(...)   EMPTY_MACRO
#define W_SYSVIEW_OnIdle(...)          EMPTY_MACRO
#define W_SYSVIEW_OnTaskStartExec(X)   EMPTY_MACRO
#define W_SYSVIEW_OnTaskStopExec(X)    EMPTY_MACRO
#define W_SYSVIEW_OnTaskCreate(X)      EMPTY_MACRO
#endif

#if USE_GRAPH
#define LOG_GRAPH(...)                 LOG_INFO(__VA_ARGS__)
#else
#define LOG_GRAPH(...)                 EMPTY_MACRO
#endif

#define SYSVIEW_MAX_NOF_TASKS          30

#define TASK_BASE                      (36u)

#define I2C_TASK                       (TASK_BASE + 0u)
#define SPI_TASK                       (TASK_BASE + 1u)
#define UART_TASK                      (TASK_BASE + 2u)

#define LCD_TASK                       (TASK_BASE + 7u)
#define SD_ACCESS_TASK                 (TASK_BASE + 8u)
#define SEG_PERF_TASK                  (TASK_BASE + 9u)
#define NRF52_TASK                     (TASK_BASE + 10u)
#define DISPLAY_TASK2                  (TASK_BASE + 11u)
#define DISPLAY_TASK3                  (TASK_BASE + 12u)
#define DISPLAY_TASK4                  (TASK_BASE + 13u)
#define USB_VCOM_TASK                  (TASK_BASE + 14u)


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
