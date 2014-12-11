/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"

#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"
#include "utils/debug.h"

#include "board/main_v3.h"

#include "ros.h"
#include <sensor_msgs/Imu.h>

#define PI 3.1415926535

using namespace stm32plus;

/* Defines -------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/

/* Constants -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

class SensorTimer {
public:
  MainV3 *mainBoard;

  sensor_msgs::Imu imu_msg;
  float yaw,pitch,roll;

  int16_t acc[3] = {0};
  int16_t gyr[3] = {0};

  SensorTimer(MainV3 *_mainBoard){
    mainBoard = _mainBoard;

    yaw=pitch=roll=0.0f;
    imu_msg.header.frame_id = "/imu";
    imu_msg.orientation.w = imu_msg.orientation.x = imu_msg.orientation.y = imu_msg.orientation.z = 0.0f;
    imu_msg.angular_velocity.x = imu_msg.angular_velocity.y = imu_msg.angular_velocity.z = 0.0f;
    imu_msg.linear_acceleration.x = imu_msg.linear_acceleration.y = imu_msg.linear_acceleration.z = 0.0f;

    timer.setTimeBaseByFrequency(10000, 100);
    // 割り込みハンドラをバインド
    timer.TimerInterruptEventSender.insertSubscriber(
	TimerInterruptEventSourceSlot::bind(this, &SensorTimer::onInterrupt)
    );
    timer.setNvicPriorities(1,0);
    timer.enableInterrupts(TIM_IT_Update); // オーバーフロー割り込み有効化
    timer.enablePeripheral();
  }

  void eulerToQuaternion(){
    float cr2 = cos(roll*0.5);
    float cp2 = cos(pitch*0.5);
    float cy2 = cos(yaw*0.5);
    float sr2 = sin(roll*0.5);
    float sp2 = sin(pitch*0.5);
    float sy2 = sin(yaw*0.5);

    imu_msg.orientation.w = cr2*cp2*cy2 + sr2*sp2*sy2;
    imu_msg.orientation.x = sr2*cp2*cy2 - cr2*sp2*sy2;
    imu_msg.orientation.y = cr2*sp2*cy2 + sr2*cp2*sy2;
    imu_msg.orientation.z = cr2*cp2*sy2 - sr2*sp2*cy2;
  }

  // 割り込みハンドラ
  void onInterrupt(TimerEventType tet,uint8_t timerNumber){
    // オーバーフロー割り込みの場合
    if(tet==TimerEventType::EVENT_UPDATE){

	mainBoard->mpu6050.readAccAll(acc);
	mainBoard->mpu6050.readGyrAll(gyr);

	const float z_offset = 0.00730;

	imu_msg.angular_velocity.x = (float)gyr[0]/32768.0f * 500.0f*(PI/180.0f);
	imu_msg.angular_velocity.y = (float)gyr[1]/32768.0f * 500.0f*(PI/180.0f);
	imu_msg.angular_velocity.z = (float)gyr[2]/32768.0f * 500.0f*(PI/180.0f) + z_offset;

	yaw += imu_msg.angular_velocity.z*0.01;
	eulerToQuaternion();

	imu_msg.linear_acceleration.x = (float)acc[0]/32768.0f *8.0f;
	imu_msg.linear_acceleration.y = (float)acc[1]/32768.0f *8.0f;
	imu_msg.linear_acceleration.z = (float)acc[2]/32768.0f *8.0f;

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

  mainBoard.mpu6050.setTimeout(200);

  //debug << "Testing MPU6050...\r\n";
  while(!mainBoard.mpu6050.test());
  //debug << "MPU6050 test passed.\r\n";

  //debug << "Setting up MPU6050...\r\n";
  mainBoard.mpu6050.setup();
  mainBoard.mpu6050.setGyrRange(mainBoard.mpu6050.GYR_RANGE_500DPS);
  //debug << "complete.\r\n";

  ros::NodeHandle  nh;
  nh.initNode();

  sensor_msgs::Imu imu_msg;
  ros::Publisher imu("imu", &imu_msg);
  nh.advertise(imu);

  SensorTimer sensor_timer(&mainBoard);
#if 1
  while(1){
      imu_msg = sensor_timer.imu_msg;
      imu.publish( &imu_msg );
      nh.spinOnce();
      MillisecondTimer::delay(25);
  }
#else
  while(1){
      char str[128];
      sprintf(str, "%.5f,\t%.5f\r\n", sensor_timer.imu_msg.angular_velocity.z, sensor_timer.yaw*180/PI);
      debug << str;
      MillisecondTimer::delay(50);
  }
#endif

}

/* End Of File ---------------------------------------------------------------*/
