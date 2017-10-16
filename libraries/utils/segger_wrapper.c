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

/*******************************************************************************
 * Functions
 ******************************************************************************/
void segger_init(void) {

#if USE_RTT
	  // RTT
	  SEGGER_RTT_Init();
#endif

#if USE_SVIEW
	  // SYSVIEW
	  SEGGER_SYSVIEW_Conf();
	  SEGGER_SYSVIEW_Start();
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
