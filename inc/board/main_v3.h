#pragma once

#ifndef USE_MAIN_V3
  #error MAIN_V3 not Selected! Select USE_MAIN_V3 in makefile
#endif

#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"
#include "config/timer.h"
#include "config/spi.h"
#include "config/display/character.h"
#include "config/button.h"

#include "utils/debug.h"

#include "hardware/canroot.h"
#include "utils/UsartInterruptWithBuffer.h"

#include "devices/led.h"
#include "devices/button.h"
#include "devices/motor.h"
#include "devices/encoder.h"
#include "devices/mcp3208.h"
#include "devices/mpu6050.h"

#include <cmath>

using namespace stm32plus;
using namespace stm32plus::display;

namespace stm32plus {

  //----------------------------------------------------------------------------------------SPI
  struct Spi3VFirstPinPackage {
    enum {
      Port_NSS=GPIOA_BASE,
      Port_SCK=GPIOA_BASE,
      Port_MISO=GPIOB_BASE,
      Port_MOSI=GPIOB_BASE,

      Pin_NSS  = GPIO_Pin_4,
      Pin_SCK  = GPIO_Pin_5,
      Pin_MISO = GPIO_Pin_4,
      Pin_MOSI = GPIO_Pin_5
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
      Port_NSS=GPIOD_BASE,
      Port_SCK=GPIOA_BASE,
      Port_MISO=GPIOB_BASE,
      Port_MOSI=GPIOB_BASE,

      Pin_NSS  = GPIO_Pin_3,
      Pin_SCK  = GPIO_Pin_5,
      Pin_MISO = GPIO_Pin_4,
      Pin_MOSI = GPIO_Pin_5
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
      Port_NSS=GPIOD_BASE,
      Port_SCK=GPIOA_BASE,
      Port_MISO=GPIOB_BASE,
      Port_MOSI=GPIOB_BASE,

      Pin_NSS  = GPIO_Pin_4,
      Pin_SCK  = GPIO_Pin_5,
      Pin_MISO = GPIO_Pin_4,
      Pin_MOSI = GPIO_Pin_5
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

  typedef Spi3VFirst<>		Spi3V1;
  typedef Spi3VSecond<> 	Spi3V2;
  typedef Spi3VThird<>  	Spi3V3;

  typedef Spi5VFirst<> 		Spi5V1;
  typedef Spi5VSecond<> 	Spi5V2;
  typedef Spi5VThird<> 		Spi5V3;

  //----------------------------------------------------------------------------------------Encoder
  using TimerEnc1 = Timer5< Timer5GpioFeature<TIMER_REMAP_NONE, TIM5_CH1_IN, TIM5_CH2_IN>, TimerEncoderFeature<EncoderCounterEdge::Inputs1And2, EncoderPolarity::Rising, EncoderPolarity::Rising> >;
  using TimerEnc2 = Timer3< Timer3GpioFeature<TIMER_REMAP_NONE, TIM3_CH1_IN, TIM3_CH2_IN>, TimerEncoderFeature<EncoderCounterEdge::Inputs1And2, EncoderPolarity::Rising, EncoderPolarity::Rising> >;
  using TimerEnc3 = Timer1< Timer1GpioFeature<TIMER_REMAP_FULL, TIM1_CH1_IN, TIM1_CH2_IN>, TimerEncoderFeature<EncoderCounterEdge::Inputs1And2, EncoderPolarity::Rising, EncoderPolarity::Rising> >;
  using TimerEnc4 = Timer8< Timer8GpioFeature<TIMER_REMAP_NONE, TIM8_CH1_IN, TIM8_CH2_IN>, TimerEncoderFeature<EncoderCounterEdge::Inputs1And2, EncoderPolarity::Rising, EncoderPolarity::Rising> >;

  class Encoders : public Encoder<TimerEnc1>, public Encoder<TimerEnc2>, public Encoder<TimerEnc3>, public Encoder<TimerEnc4> {
  public:
    Encoders():
      Encoder<TimerEnc1>(),
      Encoder<TimerEnc2>(),
      Encoder<TimerEnc3>(),
      Encoder<TimerEnc4>()
      {

      }

    ~Encoders()
    {

    }

    inline uint32_t getCounter1() const{
      return Encoder<TimerEnc1>::getCounter();
    }
    inline uint32_t getCounter2() const{
      return Encoder<TimerEnc2>::getCounter();
    }
    inline uint32_t getCounter3() const{
      return Encoder<TimerEnc3>::getCounter();
    }
    inline uint32_t getCounter4() const{
      return Encoder<TimerEnc4>::getCounter();
    }
  private:
  };

  using MOTOR_A_TIMER = Timer2<
      Timer2InternalClockFeature,
      TimerChannel1Feature<>,
      TimerChannel2Feature<>,
      Timer2GpioFeature<
        TIMER_REMAP_PARTIAL1,
        TIM2_CH1_OUT,
        TIM2_CH2_OUT
      >
  >;
  using MOTOR_A_CHA = TimerChannel1Feature<>;
  using MOTOR_A_CHB = TimerChannel2Feature<>;
  using MOTOR_A = Motor< MOTOR_A_TIMER, MOTOR_A_CHA, MOTOR_A_CHB >;

  using MOTOR_B_TIMER = Timer2<
      Timer2InternalClockFeature,
      TimerChannel3Feature<>,
      TimerChannel4Feature<>,
      Timer2GpioFeature<
      TIMER_REMAP_PARTIAL1,
      TIM2_CH3_OUT,
      TIM2_CH4_OUT
      >
  >;
  using MOTOR_B_CHA = TimerChannel3Feature<>;
  using MOTOR_B_CHB = TimerChannel4Feature<>;
  using MOTOR_B = Motor< MOTOR_B_TIMER, MOTOR_B_CHA, MOTOR_B_CHB >;

  using MOTOR_C_TIMER = Timer4<
      Timer4InternalClockFeature,
      TimerChannel3Feature<>,
      TimerChannel4Feature<>,
      Timer4GpioFeature<
      TIMER_REMAP_FULL,
      TIM4_CH3_OUT,
      TIM4_CH4_OUT
      >
  >;
  using MOTOR_C_CHA = TimerChannel3Feature<>;
  using MOTOR_C_CHB = TimerChannel4Feature<>;
  using MOTOR_C = Motor< MOTOR_C_TIMER, MOTOR_C_CHA, MOTOR_C_CHB >;

  using MOTOR_D_TIMER = Timer4<
      Timer4InternalClockFeature,
      TimerChannel1Feature<>,
      TimerChannel2Feature<>,
      Timer4GpioFeature<
      TIMER_REMAP_FULL,
      TIM4_CH1_OUT,
      TIM4_CH2_OUT
      >
  >;
  using MOTOR_D_CHA = TimerChannel1Feature<>;
  using MOTOR_D_CHB = TimerChannel2Feature<>;
  using MOTOR_D = Motor< MOTOR_D_TIMER, MOTOR_D_CHA, MOTOR_D_CHB >;

  class MainV3 {
  public:

    GpioC<DefaultDigitalOutputFeature<13> > pcled;
    Led led;

    GpioD<DefaultDigitalOutputFeature<8,9,10,11> > pdleds;
    Led led1, led2, led3, led4;

    GpioE<DigitalInputFeature <GPIO_Speed_50MHz,Gpio::PUPD_UP,1,2,3,4,12,13,14,15> > pe;
    Button button1;
    Button button2;
    Button button3;
    Button button4;

    Button toggle1;
    Button toggle2;
    Button toggle3;
    Button toggle4;

    MOTOR_A motorA;
    MOTOR_B motorB;
    MOTOR_C motorC;
    MOTOR_D motorD;


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

    CanRoot can;

    Encoders encoders;

    MCP3208 *ad[8];

    MPU6050<I2C1_Remap<I2CSingleByteMasterPollingFeature>,0> mpu6050;

#ifndef DEBUG_PORT
    UsartInterruptWithBuffer<Usart1_Remap1<Usart1InterruptFeature>, Usart1InterruptFeature> com;
    UsartInterruptWithBuffer<Usart2_Remap1<Usart2InterruptFeature>, Usart2InterruptFeature> xbee;
#endif

  public:
    MainV3() :
      led(pcled[13]),
      led1(pdleds[8]),
      led2(pdleds[9]),
      led3(pdleds[10]),
      led4(pdleds[11]),
      button1(pe[1],false),
      button2(pe[2],false),
      button3(pe[3],false),
      button4(pe[4],false),
      toggle1(pe[12],true),
      toggle2(pe[13],true),
      toggle3(pe[14],true),
      toggle4(pe[15],true),
      can(),
      encoders(),
      mpu6050(I2C::Parameters(100000)) // 100kHz
#ifndef DEBUG_PORT
      ,com()
  	  ,xbee()
#endif
  {
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
      Spi5V1Params.spi_baudRatePrescaler = SPI_BaudRatePrescaler_32;
      Spi5V1Params.spi_cpol = SPI_CPOL_Low;
      Spi5V2Params.spi_mode=SPI_Mode_Master;
      Spi5V2Params.spi_baudRatePrescaler = SPI_BaudRatePrescaler_32;
      Spi5V2Params.spi_cpol = SPI_CPOL_Low;
      Spi5V3Params.spi_mode=SPI_Mode_Master;
      Spi5V3Params.spi_baudRatePrescaler = SPI_BaudRatePrescaler_32;
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

      for(int i=0;i<8;i++){
	  ad[i] = new MCP3208(*spi5v3,i);
      }
  }


  };

}
