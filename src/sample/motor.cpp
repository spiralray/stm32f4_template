/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"

#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"
#include "utils/debug.h"

#include "board/main_v3.h"

#include "devices/spimotor.h"

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
  SpiMotor spimotor( *mainBoard.spi3v1, 0);

  MillisecondTimer::delay(100);

  while(1){
      mainBoard.led.On();
      for(int i=0; i<500;i++){
	  mainBoard.motorA.setOutput((float)i/500.0);
	  mainBoard.motorB.setOutput((float)i/500.0);
	  mainBoard.motorC.setOutput((float)i/500.0);
	  mainBoard.motorD.setOutput((float)i/500.0);
	  spimotor.setOutput((float)i/500.0);
	  MillisecondTimer::delay(10);
      }
      mainBoard.led.Off();
      for(int i=0; i>-500;i--){
	  mainBoard.motorA.setOutput((float)i/500.0);
	  mainBoard.motorB.setOutput((float)i/500.0);
	  mainBoard.motorC.setOutput((float)i/500.0);
	  mainBoard.motorD.setOutput((float)i/500.0);
	  spimotor.setOutput((float)i/500.0);
	  MillisecondTimer::delay(10);
      }
  }

  while(1);

}

/* End Of File ---------------------------------------------------------------*/
