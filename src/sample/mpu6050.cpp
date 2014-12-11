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

  MillisecondTimer::delay(100);

  MainV3 mainBoard;

  mainBoard.mpu6050.setTimeout(20);

  debug<<"Testing MPU6050...\r\n";
  while(!mainBoard.mpu6050.test());
  debug<<"MPU6050 test passed.\r\n";

  debug<<"Setting up MPU6050...\r\n";
  mainBoard.mpu6050.setup();
  debug<<"complete.\r\n";

  while(1){
      char str[128];
      sprintf(str, "Gyro / Acc: %6d %6d %6d %6d %6d %6d\r\n", mainBoard.mpu6050.readGyrX(), mainBoard.mpu6050.readGyrY(), mainBoard.mpu6050.readGyrZ(), mainBoard.mpu6050.readAccX(), mainBoard.mpu6050.readAccY(), mainBoard.mpu6050.readAccZ());
      debug << str;
      MillisecondTimer::delay(10);
  }

}

/* End Of File ---------------------------------------------------------------*/
