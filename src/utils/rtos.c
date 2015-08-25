#ifdef USE_FREERTOS

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "hardware/systick.h"


void vApplicationTickHook( void )
{

}

void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
  for( ;; );
}
void vApplicationMallocFailedHook( void ) {
  configASSERT( 0 );  // Latch on any failure / error.
}

#endif
