/*
 * led.h
 *
 *  Created on: 2014/10/05
 *      Author: spiralray
 */

#pragma once

#include "config/stm32plus.h"
#include "config/gpio.h"

namespace stm32plus{

  class Led {
  private:
    GpioPinRef pin;
    bool state;
  public:
    Led(GpioPinRef _pin){
      pin = _pin;
      state = false;
      Off();
    }

    void On() {
      pin.set();
      state = true;
    }
    void Off() {
      pin.reset();
      state = false;
    }
    void Toggle(){
    	if(state)	Off();
    	else		On();
    }
  };

}
