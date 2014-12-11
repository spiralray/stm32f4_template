#pragma once

#ifndef USE_STM32F4DISCOVERY
  #error STM32F4DISCOVERY not Selected! Select USE_STM32F4DISCOVERY in makefile
#endif

#include "config/stm32plus.h"

#include "config/gpio.h"
#include "config/spi.h"

#include "devices/led.h"
#include "devices/button.h"

using namespace stm32plus;

namespace stm32plus {

class F4Discovery {
  public:

    GpioD<DefaultDigitalOutputFeature<12,13,14,15> > pd;
    Led led1, led2, led3, led4;

    GpioA<DefaultDigitalInputFeature<0> > pa;
    Button button;

  public:
    F4Discovery() :
      led1(pd[12]), led2(pd[13]), led3(pd[14]), led4(pd[15]),
      button(pa[0],true)
    {

    }

    void test(){

    }
};

}
