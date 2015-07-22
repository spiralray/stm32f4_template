/*
 * mpu9250ahrs.cpp
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
#include "utils/MadgwickAHRS.h"

using namespace stm32plus;

/* Defines -------------------------------------------------------------------*/
#ifndef M_PI
#define M_PI 3.1415926535
#endif

#define RAD_TO_DEG (180/M_PI)

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

class SensorTimer {
public:
	MPU9250 *mpu9250;
	MadgwickAHRS ahrs;

	float yaw,pitch,roll;

	int16_t acc[3] = {0};
	int16_t gyr[3] = {0};
	float angular_velocity[3], linear_acceleration[3];

	SensorTimer(MPU9250 *_mpu9250) : ahrs()
	{
		mpu9250 = _mpu9250;

		timer.setTimeBaseByFrequency(10000, 100);
		// 割り込みハンドラをバインド
		timer.TimerInterruptEventSender.insertSubscriber(
				TimerInterruptEventSourceSlot::bind(this, &SensorTimer::onInterrupt)
		);
		timer.setNvicPriorities(1,0);
		timer.enableInterrupts(TIM_IT_Update); // オーバーフロー割り込み有効化
		timer.enablePeripheral();
	}

	// 割り込みハンドラ
	void onInterrupt(TimerEventType tet,uint8_t timerNumber){
		// オーバーフロー割り込みの場合
		if(tet==TimerEventType::EVENT_UPDATE){

			mpu9250->readAccAll(acc);
			mpu9250->readGyrAll(gyr);

			angular_velocity[0] = (float)gyr[0]/32768.0f * 500.0f*(M_PI/180.0f);
			angular_velocity[1] = (float)gyr[1]/32768.0f * 500.0f*(M_PI/180.0f);
			angular_velocity[2] = (float)gyr[2]/32768.0f * 500.0f*(M_PI/180.0f);

			linear_acceleration[0] = (float)acc[0]/32768.0f *8.0f;
			linear_acceleration[1] = (float)acc[1]/32768.0f *8.0f;
			linear_acceleration[2] = (float)acc[2]/32768.0f *8.0f;

			ahrs.update(0.01,
					angular_velocity[0], angular_velocity[1], angular_velocity[2],
					linear_acceleration[0], linear_acceleration[1], linear_acceleration[2]);

		}
	}
private:
	int count =0;
	Timer6<Timer6InternalClockFeature,Timer6InterruptFeature> timer;
};

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

	debug<<"Testing MPU6050...\r\n";
	while(!mpu9250.test());
	debug<<"MPU6050 test passed.\r\n";

	debug<<"Setting up MPU6050...\r\n";
	mpu9250.setup();
	mpu9250.setGyrRange(mpu9250.GYR_RANGE_500DPS);
	debug<<"complete.\r\n";

	SensorTimer sensor_timer(&mpu9250);

	while(1){
		char str[128];
		float yaw,pitch,roll;
		sensor_timer.ahrs.getYawPitchRoll(yaw,pitch,roll);
		sprintf(str, "YPR: %.5f %.5f %.5f\r\n", yaw*RAD_TO_DEG, pitch*RAD_TO_DEG, roll*RAD_TO_DEG);
		debug << str;

		MillisecondTimer::delay(50);
	}



}

/* End Of File ---------------------------------------------------------------*/
