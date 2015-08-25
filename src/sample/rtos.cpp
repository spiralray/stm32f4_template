/* Includes ------------------------------------------------------------------*/
/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "hardware/systick.h"

#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"

#include "board/main_v3.h"

using namespace stm32plus;

/* Defines -------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/

/* Constants -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2 )
xTaskHandle hMainTask;
portTASK_FUNCTION_PROTO( vMainTask, pvParameters );

#define SYSTICK_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2 )
xTaskHandle hSysTickTask;
portTASK_FUNCTION_PROTO( vSysTickTask, pvParameters );

/* Functions -----------------------------------------------------------------*/

portTASK_FUNCTION( vSysTickTask, pvParameters ){

	portTickType xLastWakeTime = xTaskGetTickCount();

	for(;;){
		stm32plus::MillisecondTimer::_counter++;
		vTaskDelayUntil(&xLastWakeTime, 1 );
	}
}

/**************************************************************************/
/*
    @brief  Main Program.
	@param  None.
    @retval None.
 */
/**************************************************************************/
int main(void){
	/* Set SysTickCounter for _delay_ms() and _delay_us() */
	SysTickInit(INTERVAL);

	xTaskCreate( vMainTask, (signed char *) "MAIN", configMINIMAL_STACK_SIZE*2, NULL, MAIN_TASK_PRIORITY, &hMainTask );
	xTaskCreate( vSysTickTask, (signed char *) "SYSTICK", configMINIMAL_STACK_SIZE, NULL, SYSTICK_TASK_PRIORITY, &hSysTickTask );

	vTaskStartScheduler();

	while(1);
}

portTASK_FUNCTION( vMainTask, pvParameters ){

  MainV3 board;

  while(1){
      MillisecondTimer::delay(500);
      board.led.On();
      MillisecondTimer::delay(500);
      board.led.Off();
  }

}

/* End Of File ---------------------------------------------------------------*/

