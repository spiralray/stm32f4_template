/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"

#include "config/stm32plus.h"
#include "config/timing.h"

#include "board/f4discovery.h"


using namespace stm32plus;

/* Defines -------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/

/* Constants -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/**************************************************************************/
/*!
    @brief  Main Program.
	@param  None.
    @retval None.
 */
/**************************************************************************/
int main(void)
{

  //Initialise Systick
  MillisecondTimer::initialise();

  Nvic::initialise();

  MillisecondTimer::delay(100);

  F4Discovery board;

  while(1){
      board.led1.On();
      board.led2.On();
      board.led3.Off();
      board.led4.Off();
      MillisecondTimer::delay(500);
      board.led1.Off();
      board.led2.Off();
      board.led3.On();
      board.led4.On();
      MillisecondTimer::delay(500);
  }

  return 0;

}

/* End Of File ---------------------------------------------------------------*/
