/*
 * mpu9250.cpp
 *
 *  Created on: 2015/07/22
 *      Author: spiralray
 */

/* Includes ------------------------------------------------------------------*/
#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"

#include "board/f4discovery.h"

#include "devices/mpu9250.h"

using namespace stm32plus;

/* Defines -------------------------------------------------------------------*/
struct Spi_MPU9250_PinPackage {
	enum {
		Port_NSS = GPIOC_BASE,
		Port_SCK = GPIOA_BASE,
		Port_MISO= GPIOA_BASE,
		Port_MOSI= GPIOA_BASE,

		Pin_NSS  = GPIO_Pin_4,
		Pin_SCK  = GPIO_Pin_5,
		Pin_MISO = GPIO_Pin_6,
		Pin_MOSI = GPIO_Pin_7
	};
};

template<class... Features>
struct Spi_MPU9250 : SpiPeripheral<Spi_MPU9250_PinPackage,PERIPHERAL_SPI1>,
Features... {

	Spi_MPU9250(const Parameters& params)
  			    		  : SpiPeripheral<Spi_MPU9250_PinPackage,PERIPHERAL_SPI1>(params),
							Features(static_cast<Spi&>(*this))... {
	}
};

typedef Spi_MPU9250<>		SpiMPU9250;

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

  F4Discovery board;

  SpiMPU9250::Parameters spiMPU9250_Params;
  SpiMPU9250 spiMPU9250(spiMPU9250_Params);

  MPU9250 mpu9250(spiMPU9250);

  MillisecondTimer::delay(200);

  if( !mpu9250.test() ){
	  while(1){
		  MillisecondTimer::delay(500);
		  board.led1.On();
		  MillisecondTimer::delay(500);
		  board.led1.Off();
	  }
  }

  board.led1.On();
  mpu9250.setup();

  debug<<"Setting up MPU6050...\r\n";
  mpu9250.setup();
  debug<<"complete.\r\n";

  while(1){
	  char str[128];
	  int16_t gyr[3];
	  int16_t acc[3];
#if 0
	  sprintf(str, "Gyro / Acc: %6d %6d %6d %6d %6d %6d\r\n", mpu9250.readGyrX(), mpu9250.readGyrY(), mpu9250.readGyrZ(), mpu9250.readAccX(), mpu9250.readAccY(), mpu9250.readAccZ());
#else
	  mpu9250.readAccAll(acc);
	  mpu9250.readGyrAll(gyr);
	  sprintf(str, "Gyro / Acc: %6hd %6hd %6hd %6hd %6hd %6hd\r\n", gyr[0], gyr[1], gyr[2], acc[0], acc[1], acc[2]);
#endif
	  debug << str;
	  MillisecondTimer::delay(10);
  }



}

/* End Of File ---------------------------------------------------------------*/

