/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"

#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"
#include "utils/debug.h"
#include "board/main_v3.h"

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

  MainV3 mainBoard;

  //Test code for MCP3208
  while(1){
      char str[128];
      sprintf(str, "AD: %5d %5d %5d %5d %5d %5d %5d %5d", mainBoard.ad[0]->get(), mainBoard.ad[1]->get(), mainBoard.ad[2]->get(), mainBoard.ad[3]->get(), mainBoard.ad[4]->get(), mainBoard.ad[5]->get(), mainBoard.ad[6]->get(), mainBoard.ad[7]->get());
      debug << str << "\r\n";
      MillisecondTimer::delay(100);
  }

}

/* End Of File ---------------------------------------------------------------*/
