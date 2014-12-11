/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"

#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"
#include "utils/debug.h"

#include "board/main_v3.h"

#include "utils/MadgwickAHRS.h"

using namespace stm32plus;

/* Defines -------------------------------------------------------------------*/
#ifndef M_PI
  #define M_PI 3.1415926535
#endif

#define RAD_TO_DEG (180/M_PI)

/* Variables -----------------------------------------------------------------*/

/* Constants -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/


class SensorTimer {
public:
  MainV3 *mainBoard;
  MadgwickAHRS ahrs;

  float yaw,pitch,roll;

  int16_t acc[3] = {0};
  int16_t gyr[3] = {0};
  float angular_velocity[3], linear_acceleration[3];

  SensorTimer(MainV3 *_mainBoard) : ahrs()
  {
    mainBoard = _mainBoard;

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

	mainBoard->mpu6050.readAccAll(acc);
	mainBoard->mpu6050.readGyrAll(gyr);

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
  mainBoard.mpu6050.setGyrRange(mainBoard.mpu6050.GYR_RANGE_500DPS);
  debug<<"complete.\r\n";

  SensorTimer sensor_timer(&mainBoard);

  while(1){
      float q[4];
      sensor_timer.ahrs.getQuaternion(q);
      char str[128];
      sprintf(str, "Q: %.5f %.5f %.5f %.5f\r\n", q[0], q[1], q[2], q[3] );
      debug << str;

      float yaw,pitch,roll;
      sensor_timer.ahrs.getYawPitchRoll(yaw,pitch,roll);
      sprintf(str, "YPR: %.5f %.5f %.5f\r\n", yaw*RAD_TO_DEG, pitch*RAD_TO_DEG, roll*RAD_TO_DEG);
      debug << str;

      MillisecondTimer::delay(50);
  }

}

/* End Of File ---------------------------------------------------------------*/
