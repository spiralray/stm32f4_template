/* Includes ------------------------------------------------------------------*/
#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"

#include "board/main_v3.h"

using namespace stm32plus;

/* Defines -------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/

/* Constants -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/**************************************************************************/
/*
    @brief  Main Program.
	@param  None.
    @retval None.
 */
/**************************************************************************/
int main(void)
{
  //Initialise Systick
  MillisecondTimer::initialise();

  MainV3 board;

  while(1){
      MillisecondTimer::delay(500);
      board.led.On();
      MillisecondTimer::delay(500);
      board.led.Off();
  }

}

/* End Of File ---------------------------------------------------------------*/

