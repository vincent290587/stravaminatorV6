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


#if USE_RTT
#define LOG_INFO(...)               SEGGER_RTT_printf(RTT_LOG_CHANNEL, __VA_ARGS__)
#else
#define LOG_INFO(...)               do {} while (0)
#endif

#if USE_SVIEW
#define W_SYSVIEW_OnIdle(...)       SEGGER_SYSVIEW_OnIdle()
#else
#define W_SYSVIEW_OnIdle(...)       do {} while (0)
#endif


//SEGGER_SYSVIEW_MODULE IPModule = {
//		"M=embOSIP, " \
//		"0 SendPacket IFace=%u NumBytes=%u, " \
//		"1 ReceivePacket Iface=%d NumBytes=%u", // sModule
//		2, // NumEvents
//		0,
//		// EventOffset, Set by SEGGER_SYSVIEW_RegisterModule()
//		NULL,
//		// pfSendModuleDesc, NULL: No additional module description
//		NULL,
//		// pNext, Set by SEGGER_SYSVIEW_RegisterModule()
//};
//SEGGER_SYSVIEW_RegisterModule(&IPModule);


//SEGGER_SYSVIEW_RecordU32x2(
// // Id of SendPacket (0) + Offset for the registered module
// ID_SENDPACKET + IPModule.EventOffset,
// // First parameter (displayed as event parameter IFace)
// pPacket->Interface,
// // Second parameter (displayed as event parameter NumBytes)
// pPacket->NumBytes
// );


/////////    FUNCTIONS

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

void segger_init(void);

#if defined(__cplusplus)
}
#endif /* _cplusplus */

#endif /* LIBRARIES_UTILS_SEGGER_WRAPPER_H_ */
