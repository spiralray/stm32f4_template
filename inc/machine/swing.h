#pragma once

/* Includes ------------------------------------------------------------------*/
#include "config/stm32plus.h"

#include "devices/button.h"
#include "devices/motor.h"

/* Defines -------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/

/* Constants -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* Classes -----------------------------------------------------------------*/

template<class TMotorA, class TMotorB, class TEncoder>
class Swing : public TMotorA, public TMotorB, public TEncoder {
protected:
  Button *buttonSwing, *buttonRevert;
public:
  Swing( Button *_buttonSwing, Button *_buttonRevert ) :TMotorA(), TMotorB(), TEncoder()
  {
    buttonSwing = _buttonSwing;
    buttonRevert = _buttonRevert;
  }

  void update(){
    TEncoder::updateLocation();

    float power = 0.0f;

    if(buttonSwing->getState() == true){
	if( abs(TEncoder::getLocation()) < 3250){
	    power = -1.0f;
	}
	else{
	    power = 0.0f;
	}
    }
    else if( TEncoder::getLocation() < -10 && buttonRevert->getState() == true){
	power = 0.04f;
    }
    else{
	power = 0.0f;
    }

    TMotorA::setOutput(power);
    TMotorB::setOutput(-power);
  }
};
