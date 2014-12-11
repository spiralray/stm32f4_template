/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"

#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"
#include "utils/debug.h"

#include "board/main_v3.h"

#include "machine/machine.h"

#include "ros.h"
#include <std_msgs/Int32.h>


using namespace stm32plus;

/* Defines -------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/
ros::NodeHandle  *nh;
Machine *machine;

/* Constants -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
void motor1CallBack(const std_msgs::Int32& msg){
  machine->setTargetLocation1(msg.data);
}
ros::Subscriber<std_msgs::Int32> subMotor1("motor1", &motor1CallBack);

void motor2CallBack(const std_msgs::Int32& msg){
  machine->setTargetLocation2(msg.data);
}
ros::Subscriber<std_msgs::Int32> subMotor2("motor2", &motor2CallBack);

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

  machine = new Machine();	//You must call before initialization of NodeHandle

  nh = new ros::NodeHandle();

  MillisecondTimer::delay(100);

  std_msgs::Int32 encoder1_msg, encoder2_msg, encoderSwing_msg;
  encoder1_msg.data = machine->motorA.Encoder::getLocation();
  encoder2_msg.data = machine->motorB.Encoder::getLocation();
  encoderSwing_msg.data = machine->swing.Encoder::getLocation();
  ros::Publisher encoder1("enc1", &encoder1_msg);
  ros::Publisher encoder2("enc2", &encoder1_msg);
  ros::Publisher encoderSwing("encSwing", &encoderSwing_msg);

  nh->initNode();
  nh->advertise(encoder1);
  nh->advertise(encoder2);
  nh->advertise(encoderSwing);

  nh->subscribe(subMotor1);
  nh->subscribe(subMotor2);

  while(1){
	  encoder1_msg.data = machine->motorA.Encoder::getLocation();
	  encoder1.publish( &encoder1_msg );

	  encoder2_msg.data = machine->motorB.Encoder::getLocation();
	  encoder2.publish( &encoder2_msg );

	  encoderSwing_msg.data = machine->swing.Encoder::getLocation();
	  encoderSwing.publish( &encoderSwing_msg );

      nh->spinOnce();

      MillisecondTimer::delay(10);
      machine->led.On();
      MillisecondTimer::delay(10);
      machine->led.Off();
  }

}

/* End Of File ---------------------------------------------------------------*/

