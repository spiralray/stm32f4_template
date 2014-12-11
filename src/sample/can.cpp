/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"

#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"
#include "utils/debug.h"

#include "board/main_v3.h"

#include "devices/can/ps3con.h"
#include "devices/can/canair.h"

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

  MillisecondTimer::delay(100);

  CanAir canair(&mainBoard.can);

  canair.Set(0);
  canair.Update();
  MillisecondTimer::delay(1000);
  canair.Reset(0);
  canair.Update();

  PS3Con *ps3con = new PS3Con();

  mainBoard.can.AddListenerNode(ps3con);

  while(1){
      mainBoard.can.Update();
      if(ps3con->getButtonPress(CROSS)){
	  debug << "pressed\r\n";
      }
      else{
	  debug << "not pressed\r\n";
      }
      MillisecondTimer::delay(50);
  }

  while(1);

}

/* End Of File ---------------------------------------------------------------*/
