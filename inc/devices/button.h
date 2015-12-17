/*
 * button.h
 *
 *  Created on: 2014/11/21
 *      Author: spiralray
 */

#pragma once

#include "config/stm32plus.h"
#include "config/gpio.h"

namespace stm32plus{

  class Button {
  private:
    GpioPinRef pin;
    bool pushedstate;
  public:
    Button(const GpioPinRef& _pin, bool _pushedstate=false) : pin(_pin){
      pushedstate = _pushedstate;
    }

    bool getState(){
      return ( pin.read() == pushedstate);
    }
  };

}
