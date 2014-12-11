#pragma once

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

#include "config/stm32plus.h"
#include "board/main_v3.h"

#include "control/controlTimer.h"
#include "control/motorcontroller.h"

#include "machine/swing.h"

#include "utils/debug.h"

/* Defines -------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/

/* Constants -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* Classes -----------------------------------------------------------------*/

using namespace stm32plus;

class Machine : public MainV3 {
private:
  int32_t targetLocation1, targetLocation2;

public:
//private:
  MotorLocationController<
    float,
    Motor< MOTOR_A_TIMER, MOTOR_A_CHB, MOTOR_A_CHA >,
    Encoder<TimerEnc1>
  > motorA;

  MotorLocationController<
    float,
    Motor< MOTOR_B_TIMER, MOTOR_B_CHA, MOTOR_B_CHB >,
    Encoder<TimerEnc2>
  > motorB;

  Swing<
    Motor< MOTOR_C_TIMER, MOTOR_C_CHA, MOTOR_C_CHB >,
    Motor< MOTOR_D_TIMER, MOTOR_D_CHA, MOTOR_D_CHB >,
    Encoder<TimerEnc3>
  > swing;


  ControlTimer<
  Timer7< Timer7InternalClockFeature, Timer7InterruptFeature >,
  Machine
  > mainCtrlTimer;

  uint32_t count = 0;

public:
  Machine(): MainV3(),
  motorA(0.005f, 0.002000, 0.00001, 0.001f, 1.0f, 2.0, 500, 20),	//P, I, D, Max duty ratio
  motorB(0.012f, 0.001, 0.000005f, 0.002f, 1.0f, 0.7, 100, 100),
  swing(&button1, &button2),
  mainCtrlTimer(this,1000,2) // 1000Hz = 1ms cycle. priority:2
  {
      motorA.setOutput(0.0f);
      motorB.setOutput(0.0f);

      targetLocation1 = 0;
      targetLocation2 = 0;

      mainCtrlTimer.bind(&Machine::ctrlHandler);
  }

  void ctrlHandler(TimerEventType te, uint8_t timerNumber){
	  motorA.Encoder::updateLocation();
	  motorB.Encoder::updateLocation();

	  if( abs(motorA.Encoder::getLocation()) < 15000){
		  motorA.setTargetLocation( getTargetLocation1() );
	  }
	  else{
		  motorA.emergencyStop();
		  motorB.emergencyStop();
	  }

#if 0
	  char str[128];
	  sprintf(str, "%5d,%5d,%6u", (int)motorA.getNowTargetSpeed(), (int)motorA.Encoder::getSpeed(), (unsigned int)motorA.Encoder::getLocation());
	  debug << str << "\r\n";
#endif

	  if( abs(motorB.Encoder::getLocation()) < 7500){
		  motorB.setTargetLocation( getTargetLocation2() );
	  }
	  else{
		  motorA.emergencyStop();
		  motorB.emergencyStop();
	  }

	  motorA.update();
	  motorB.update();

	  //swing.update();

	  count++;

#if 0
	  debug << getTargetLocation1() << "\r\n";
	  debug.flush();
#endif
  }

  void setTargetLocation1( int32_t target ){
    targetLocation1 = target;
  }

  int32_t getTargetLocation1(){
    return targetLocation1;
  }

  void setTargetLocation2( int32_t target ){
    targetLocation2 = target;
  }

  int32_t getTargetLocation2(){
    return targetLocation2;
  }

};
