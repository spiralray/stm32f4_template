#pragma once

#ifndef USE_STM32F4MINI
  #error USE_STM32F4MINI not Selected! Select USE_STM32F4MINI in makefile
#endif

#include "config/stm32plus.h"

#include "config/gpio.h"

#include "devices/led.h"
#include "devices/button.h"

using namespace stm32plus;

namespace stm32plus {

class F4Mini {
  public:

    GpioD<DefaultDigitalOutputFeature<2> > pd;
    Led led;

    GpioA<DefaultDigitalInputFeature<0> > pa;
    Button button;

  public:
    F4Mini() :
      led(pd[2]),
      button(pa[0],true)
    {

    }

};

}
