#pragma once

#ifndef USE_MAIN_V2
  #error MAIN_V2 not Selected! Select USE_MAIN_V2 in makefile
#endif

#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"
#include "config/timer.h"
#include "config/spi.h"
#include "config/display/character.h"

#include "utils/debug.h"

#include "devices/led.h"

#include "devices/motor.h"

#include <cmath>

using namespace stm32plus;
using namespace stm32plus::display;

namespace stm32plus {
  struct Spi3VFirstPinPackage {
    enum {
      Port_NSS=GPIOA_BASE,
      Port_SCK=GPIOA_BASE,
      Port_MISO=GPIOA_BASE,
      Port_MOSI=GPIOA_BASE,

      Pin_NSS  = GPIO_Pin_4,
      Pin_SCK  = GPIO_Pin_5,
      Pin_MISO = GPIO_Pin_6,
      Pin_MOSI = GPIO_Pin_7
    };
  };

  template<class... Features>
  struct Spi3VFirst : SpiPeripheral<Spi3VFirstPinPackage,PERIPHERAL_SPI1>,
  Features... {

    Spi3VFirst(const Parameters& params)
	      : SpiPeripheral<Spi3VFirstPinPackage,PERIPHERAL_SPI1>(params),
	        Features(static_cast<Spi&>(*this))... {
    }
  };

  struct Spi3VSecondPinPackage {
    enum {
      Port_NSS=GPIOC_BASE,
      Port_SCK=GPIOA_BASE,
      Port_MISO=GPIOA_BASE,
      Port_MOSI=GPIOA_BASE,

      Pin_NSS  = GPIO_Pin_4,
      Pin_SCK  = GPIO_Pin_5,
      Pin_MISO = GPIO_Pin_6,
      Pin_MOSI = GPIO_Pin_7
    };
  };

  template<class... Features>
  struct Spi3VSecond : SpiPeripheral<Spi3VSecondPinPackage,PERIPHERAL_SPI1>,
  Features... {

    Spi3VSecond(const Parameters& params)
  		  : SpiPeripheral<Spi3VSecondPinPackage,PERIPHERAL_SPI1>(params),
  		    Features(static_cast<Spi&>(*this))... {
    }
  };

  struct Spi3VThirdPinPackage {
    enum {
      Port_NSS=GPIOC_BASE,
      Port_SCK=GPIOA_BASE,
      Port_MISO=GPIOA_BASE,
      Port_MOSI=GPIOA_BASE,

      Pin_NSS  = GPIO_Pin_5,
      Pin_SCK  = GPIO_Pin_5,
      Pin_MISO = GPIO_Pin_6,
      Pin_MOSI = GPIO_Pin_7
    };
  };

  template<class... Features>
  struct Spi3VThird : SpiPeripheral<Spi3VThirdPinPackage,PERIPHERAL_SPI1>,
  Features... {

    Spi3VThird(const Parameters& params)
  		  : SpiPeripheral<Spi3VThirdPinPackage,PERIPHERAL_SPI1>(params),
  		    Features(static_cast<Spi&>(*this))... {
    }
  };

  struct Spi5VFirstPinPackage {
    enum {
      Port_NSS=GPIOB_BASE,
      Port_SCK=GPIOB_BASE,
      Port_MISO=GPIOB_BASE,
      Port_MOSI=GPIOB_BASE,

      Pin_NSS  = GPIO_Pin_10,
      Pin_SCK  = GPIO_Pin_13,
      Pin_MISO = GPIO_Pin_14,
      Pin_MOSI = GPIO_Pin_15
    };
  };

  template<class... Features>
  struct Spi5VFirst : SpiPeripheral<Spi5VFirstPinPackage,PERIPHERAL_SPI2>,
  Features... {

    Spi5VFirst(const Parameters& params)
  		  : SpiPeripheral<Spi5VFirstPinPackage,PERIPHERAL_SPI2>(params),
  		    Features(static_cast<Spi&>(*this))... {
    }
  };

  struct Spi5VSecondPinPackage {
    enum {
      Port_NSS=GPIOB_BASE,
      Port_SCK=GPIOB_BASE,
      Port_MISO=GPIOB_BASE,
      Port_MOSI=GPIOB_BASE,

      Pin_NSS  = GPIO_Pin_11,
      Pin_SCK  = GPIO_Pin_13,
      Pin_MISO = GPIO_Pin_14,
      Pin_MOSI = GPIO_Pin_15
    };
  };

  template<class... Features>
  struct Spi5VSecond : SpiPeripheral<Spi5VSecondPinPackage,PERIPHERAL_SPI2>,
  Features... {

    Spi5VSecond(const Parameters& params)
  		  : SpiPeripheral<Spi5VSecondPinPackage,PERIPHERAL_SPI2>(params),
  		    Features(static_cast<Spi&>(*this))... {
    }
  };

  struct Spi5VThirdPinPackage {
    enum {
      Port_NSS=GPIOB_BASE,
      Port_SCK=GPIOB_BASE,
      Port_MISO=GPIOB_BASE,
      Port_MOSI=GPIOB_BASE,

      Pin_NSS  = GPIO_Pin_12,
      Pin_SCK  = GPIO_Pin_13,
      Pin_MISO = GPIO_Pin_14,
      Pin_MOSI = GPIO_Pin_15
    };
  };

  template<class... Features>
  struct Spi5VThird : SpiPeripheral<Spi5VThirdPinPackage,PERIPHERAL_SPI2>,
  Features... {

    Spi5VThird(const Parameters& params)
    		  : SpiPeripheral<Spi5VThirdPinPackage,PERIPHERAL_SPI2>(params),
    		    Features(static_cast<Spi&>(*this))... {
    }
  };

}

typedef Spi3VFirst<>	Spi3V1;
typedef Spi3VSecond<> 	Spi3V2;
typedef Spi3VThird<>  	Spi3V3;

typedef Spi5VFirst<> 	Spi5V1;
typedef Spi5VSecond<> 	Spi5V2;
typedef Spi5VThird<> 	Spi5V3;

class MainV2 {
  public:

	GpioB<DigitalOutputFeature< GPIO_Speed_50MHz, Gpio::OPEN_DRAIN, Gpio::PUPD_NONE, 0> > pb_od;
    Led led;

    Motor<
      Timer2<
        Timer2InternalClockFeature,
        TimerChannel1Feature<>,
        TimerChannel2Feature<>,
        Timer2GpioFeature<
          TIMER_REMAP_NONE,
          TIM2_CH1_OUT,
          TIM2_CH2_OUT
        >
      >,
      TimerChannel1Feature<>, TimerChannel2Feature<>
    > motorA;

    Motor<
      Timer2<
        Timer2InternalClockFeature,
        TimerChannel3Feature<>,
        TimerChannel4Feature<>,
        Timer2GpioFeature<
          TIMER_REMAP_NONE,
          TIM2_CH3_OUT,
          TIM2_CH4_OUT
        >
      >,
      TimerChannel3Feature<>, TimerChannel4Feature<>
    > motorB;

    Motor<
      Timer3<
        Timer3InternalClockFeature,
        TimerChannel1Feature<>,
        TimerChannel2Feature<>,
        Timer3GpioFeature<
          TIMER_REMAP_FULL,
          TIM3_CH1_OUT,
          TIM3_CH2_OUT
        >
      >,
      TimerChannel1Feature<>, TimerChannel2Feature<>
    > motorC;

    Motor<
      Timer4<
        Timer4InternalClockFeature,
        TimerChannel1Feature<>,
        TimerChannel2Feature<>,
        Timer4GpioFeature<
          TIMER_REMAP_NONE,
          TIM4_CH1_OUT,
          TIM4_CH2_OUT
        >
      >,
      TimerChannel1Feature<>, TimerChannel2Feature<>
    > motorD;

    GpioD<DefaultDigitalOutputFeature<0,1,3,4,5,6> > pd;
    HD44780 lcd;
    CharacterLcdTerminal<HD44780> display;

    Spi3V1::Parameters Spi3V1Params;
    Spi3V2::Parameters Spi3V2Params;
    Spi3V3::Parameters Spi3V3Params;

    Spi5V1::Parameters Spi5V1Params;
    Spi5V2::Parameters Spi5V2Params;
    Spi5V3::Parameters Spi5V3Params;

    Spi3V1 *spi3v1;
    Spi3V2 *spi3v2;
    Spi3V3 *spi3v3;

    Spi5V1 *spi5v1;
    Spi5V2 *spi5v2;
    Spi5V3 *spi5v3;

  public:
    MainV2() :
      led(pb_od[0]),
      lcd(pd[0],pd[1],pd[3],pd[4],pd[5],pd[6],16,2),
      display(lcd)
    {

      Spi1<>::Parameters;
      Spi3V1Params.spi_mode=SPI_Mode_Master;
      Spi3V1Params.spi_baudRatePrescaler = SPI_BaudRatePrescaler_256;
      Spi3V1Params.spi_cpol = SPI_CPOL_Low;
      Spi3V2Params.spi_mode=SPI_Mode_Master;
      Spi3V2Params.spi_baudRatePrescaler = SPI_BaudRatePrescaler_256;
      Spi3V2Params.spi_cpol = SPI_CPOL_Low;
      Spi3V3Params.spi_mode=SPI_Mode_Master;
      Spi3V3Params.spi_baudRatePrescaler = SPI_BaudRatePrescaler_256;
      Spi3V3Params.spi_cpol = SPI_CPOL_Low;

      Spi5V1Params.spi_mode=SPI_Mode_Master;
      Spi5V1Params.spi_baudRatePrescaler = SPI_BaudRatePrescaler_256;
      Spi5V1Params.spi_cpol = SPI_CPOL_Low;
      Spi5V2Params.spi_mode=SPI_Mode_Master;
      Spi5V2Params.spi_baudRatePrescaler = SPI_BaudRatePrescaler_256;
      Spi5V2Params.spi_cpol = SPI_CPOL_Low;
      Spi5V3Params.spi_mode=SPI_Mode_Master;
      Spi5V3Params.spi_baudRatePrescaler = SPI_BaudRatePrescaler_256;
      Spi5V3Params.spi_cpol = SPI_CPOL_Low;


      spi3v1 = new Spi3V1(Spi3V1Params);
      spi3v2 = new Spi3V2(Spi3V2Params);
      spi3v3 = new Spi3V3(Spi3V3Params);
      spi5v1 = new Spi5V1(Spi5V1Params);
      spi5v2 = new Spi5V2(Spi5V2Params);
      spi5v3 = new Spi5V3(Spi5V3Params);

      spi3v1->setNss(true);
      spi3v2->setNss(true);
      spi3v3->setNss(true);
      spi5v1->setNss(true);
      spi5v2->setNss(true);
      spi5v3->setNss(true);

      display.clear();
    }

    void test(){

    }
};

