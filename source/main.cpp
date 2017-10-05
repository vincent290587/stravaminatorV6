/**
 * This is template for main module created by MCUXpresso Project Generator. Enjoy!
 **/

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "Segment.h"
#include "segger_wrapper.h"
#include "ls027.h"
#include "composite_public.h"
#include "millis.h"


/*!
 * @brief Application entry point.
 */
int main(void) {
  /* Init board hardware. */
  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  BOARD_InitDebugConsole();

  /* Init code */
  millis_init();

  // LCD driver
  LS027_Init();

  // USB driver
  CompositeInit();

  // Segger
  segger_init();


  for(;;) { /* Infinite loop to avoid leaving the main function */

	delay_ms(100); /* something to use as a breakpoint stop while looping */

    CompositeTask();

    LOG_INFO("Hello World millis= %lu\r\n", millis());

  }

  return 0;
}


