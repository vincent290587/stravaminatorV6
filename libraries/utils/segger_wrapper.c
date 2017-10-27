/*
 * segger_wrapper.c
 *
 *  Created on: 5 oct. 2017
 *      Author: Vincent
 */


#include "segger_wrapper.h"


/*******************************************************************************
 * Variables
 ******************************************************************************/
/* System clock frequency. */
extern uint32_t SystemCoreClock;

SEGGER_SYSVIEW_OS_API os_api;

#if USE_SVIEW
static SEGGER_SYSVIEW_TASKINFO pInfo[SYSVIEW_MAX_NOF_TASKS];
static uint32_t nb_tasks;
#endif

/*******************************************************************************
 * Functions
 ******************************************************************************/
void cbSendTaskList(void) {

#if USE_SVIEW

	for (int i = 0; i < nb_tasks; i++) {

		SEGGER_SYSVIEW_SendTaskInfo(&pInfo[i]);

	}

#endif

}


void segger_init(void) {

#if USE_RTT && !USE_SVIEW
	  // RTT
	  SEGGER_RTT_Init();
#endif

#if USE_SVIEW
	  // SYSVIEW
	  os_api.pfSendTaskList = cbSendTaskList;
//	  os_api.pfGetTime = (U64)millis();
	  os_api.pfGetTime = 0;

	  nb_tasks = 0;

	  pInfo[nb_tasks].TaskID = I2C_TASK;
	  pInfo[nb_tasks++].sName  = "I2C_TASK";

	  pInfo[nb_tasks].TaskID = UART_TASK;
	  pInfo[nb_tasks++].sName  = "UART_TASK";

	  pInfo[nb_tasks].TaskID = SPI_TASK;
	  pInfo[nb_tasks++].sName  = "SPI_TASK";

	  pInfo[nb_tasks].TaskID = SD_ACCESS_TASK;
	  pInfo[nb_tasks++].sName  = "SD_ACCESS_TASK";

	  pInfo[nb_tasks].TaskID = SEG_PERF_TASK;
	  pInfo[nb_tasks++].sName  = "SEG_PERF_TASK";

	  pInfo[nb_tasks].TaskID = LCD_TASK;
	  pInfo[nb_tasks++].sName  = "LCD_TASK";

	  SEGGER_SYSVIEW_Conf();

	  SEGGER_SYSVIEW_Start();

	  for (int i = 0; i < nb_tasks; i++) {

		  SEGGER_SYSVIEW_OnTaskCreate(pInfo[i].TaskID);

	  }

#endif

}

void segger_update_clocks() {
#if USE_SVIEW
	SEGGER_SYSVIEW_UpdateClocks(SystemCoreClock, SystemCoreClock);
#endif
}

void segger_send(UART_Type* base, const uint8_t* buffer, size_t length) {
#if USE_RTT

	SEGGER_RTT_Write(RTT_LOG_CHANNEL, buffer, (unsigned) length);

#endif
}

status_t segger_recv(UART_Type* base, uint8_t* buffer, size_t length) {
#if USE_RTT

	unsigned read_bytes = 0;

	read_bytes = SEGGER_RTT_Read(RTT_LOG_CHANNEL, buffer + read_bytes, length);

	if (!read_bytes) return 1;

#endif

	return 0;
}
