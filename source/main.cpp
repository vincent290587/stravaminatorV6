/**
 * This is template for main module created by MCUXpresso Project Generator. Enjoy!
 **/

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "Segment.h"

#include "SEGGER_SYSVIEW.h"
#include "SEGGER_RTT.h"

extern "C" void CompositeInit(void);
extern "C" void CompositeTask(void);

void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 800000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

/*!
 * @brief Application entry point.
 */
int main(void) {
  /* Init board hardware. */
  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  BOARD_InitDebugConsole();

  /* Init code here */

  // USB
  CompositeInit();

  // RTT
  SEGGER_RTT_Init();

  // SYSVIEW
  SEGGER_SYSVIEW_Conf();
  SEGGER_SYSVIEW_Start();


  for(;;) { /* Infinite loop to avoid leaving the main function */

	SEGGER_SYSVIEW_OnIdle();

	delay(); /* something to use as a breakpoint stop while looping */

    CompositeTask();

    //SEGGER_RTT_WriteString(1, "Hello World from SEGGER!\r\n");

  }

  return 0;
}


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
