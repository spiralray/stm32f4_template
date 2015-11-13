/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"

#include "board/f4mini.h"

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

  F4Mini board;

  while(1){
	  board.led.On();
	  MillisecondTimer::delay(500);
	  board.led.Off();
	  MillisecondTimer::delay(500);
  }

}

/* End Of File ---------------------------------------------------------------*/

