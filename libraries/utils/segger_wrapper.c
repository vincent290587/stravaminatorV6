/*
 * segger_wrapper.c
 *
 *  Created on: 5 oct. 2017
 *      Author: Vincent
 */

#include "segger_wrapper.h"


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

