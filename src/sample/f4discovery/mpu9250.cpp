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

  while(1){

  }



}

/* End Of File ---------------------------------------------------------------*/

