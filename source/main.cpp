/**
 * This is template for main module created by MCUXpresso Project Generator. Enjoy!
 **/

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "Segment.h"

extern "C" void CompositeInit(void);
extern "C" void CompositeTask(void);

/*!
 * @brief Application entry point.
 */
int main(void) {
  /* Init board hardware. */
  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  BOARD_InitDebugConsole();

  /* Add your code here */
  CompositeInit();

  for(;;) { /* Infinite loop to avoid leaving the main function */


    __asm("NOP"); /* something to use as a breakpoint stop while looping */

    CompositeTask();

  }

  return 0;
}
