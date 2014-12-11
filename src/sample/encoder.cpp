/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"

#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"
#include "utils/debug.h"

#include "board/main_v3.h"
#include "devices/spiencoder.h"

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

  SpiEncoder spiencoder1( *mainBoard.spi3v1, 0);
  SpiEncoder spiencoder2( *mainBoard.spi3v1, 1);

  MillisecondTimer::delay(100);

#if 1
  //Test code for Encoder Timer(for MainV3 only)
  char str[64];
  while(1){
      sprintf(str, "%5d %5d %5d %5d\r\n", mainBoard.encoders.getCounter1(), mainBoard.encoders.getCounter2(), mainBoard.encoders.getCounter3(), mainBoard.encoders.getCounter4());
      debug << str;
      MillisecondTimer::delay(100);
  }
#else
  //Test code for SPI Encoder
  uint16_t enc;
  while(1){
      char str[64];

      enc = spiencoder1.getCounter();
      sprintf(str, "%5d", enc);
      debug << enc << "\t";

      enc = spiencoder2.getCounter();
      sprintf(str, "%5d", enc);
      debug << enc << "\r\n";
      MillisecondTimer::delay(100);
  }
#endif

}

/* End Of File ---------------------------------------------------------------*/
