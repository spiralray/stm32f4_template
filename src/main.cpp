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

  nh = new ros::NodeHandle();

  MillisecondTimer::delay(100);

  machine = new Machine();

  std_msgs::Int32 encoder1_msg;
  encoder1_msg.data = 0;
  ros::Publisher encoder1("enc1", &encoder1_msg);

  nh->initNode();
  nh->advertise(encoder1);
  nh->subscribe(subMotor1);

  //Nvic::configureIrq(USART2_IRQn,ENABLE,0,0);
  //Nvic::configureIrq(TIMER7_IRQn,ENABLE,2,0);

  while(1){
      encoder1_msg.data = machine->motorA.Encoder::getLocation();
      encoder1.publish( &encoder1_msg );
      nh->spinOnce();
      MillisecondTimer::delay(50);
      machine->led.On();
      MillisecondTimer::delay(50);
      machine->led.Off();
  }

}

/* End Of File ---------------------------------------------------------------*/

