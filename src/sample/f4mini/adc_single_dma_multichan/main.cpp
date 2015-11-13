/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timing.h"
#include "config/adc.h"
#include "config/usart.h"
#include "config/string.h"

#include "devices/led.h"
#include "utils/debug.h"

using namespace stm32plus;


/**
 * This example adds DMA and multi-channel conversion to the mix. We'll use ADC1 to convert
 * three channels plus the internal temperature automatically and in sequence and we'll
 * write out the results to the USART for you to see.
 * On the F4:
 *   The ADC is configured in 'scan mode' which means that it will convert all the
 *   configured channels and, because we are not using continuous mode, it will stop at the end
 *   of the group.
 *
 * The DMA channel for ADC1 is used to move the converted channel data out to SRAM.
 * We configure the 'complete' DMA interrupt to fire when the complete group has
 * finished converting. The converted data is written to the USART, we pause for a second
 * and then do it all again, ad infinitum.
 *
 * USART1 is configured with protocol settings of 57600/8/N/1. The ADC channels are read
 * from PA[0], PA[1]. You will need to connect these GPIO inputs to valid levels
 * between GND and VREF to see conversion values.
 */


class AdcSingleDmaMultiChan {

  private:
    volatile bool _ready;

  public:

    void run() {

      _ready=false;

      /*
       * We're converting 2 channels in circular buffer mode so we need exactly 4 16-bit words
       * for that circular buffer
       */

      volatile uint16_t readBuffer[2];

      GpioD<DefaultDigitalOutputFeature<2> > pd;
      Led led(pd[2]);

      /*
       * Declare the ADC1 DMA channel. The default is circular mode for the AdcDmaFeature
       * which means that it wil automatically refill our buffer on each conversion because
       * one conversion exactly matches the size of the memory buffer that we will give
       * to the DMA peripheral.
       */

      Adc1DmaChannel<AdcDmaFeature<Adc1PeripheralTraits>,Adc1DmaChannelInterruptFeature> dma;

      /*
       * Declare the ADC peripheral with an APB2 clock prescaler of 2, a resolution of
       * 12 bits. We will use 144-cycle conversions on ADC channels 0,1
       * that causes EOC to be raised at the end of a complete conversion group.
       */

      Adc1<
        AdcClockPrescalerFeature<2>,                // prescaler of 2
        AdcResolutionFeature<12>,                   // 12 bit resolution
        Adc1Cycle144RegularChannelFeature<0,1>,     // using channels 0,1 on ADC1 with 144-cycle latency
        AdcScanModeFeature<>                        // scan mode with EOC after each group
      > adc;

      /*
       * Subscribe to the DMA complete interrupt
       */

      dma.DmaInterruptEventSender.insertSubscriber(
          DmaInterruptEventSourceSlot::bind(this,&AdcSingleDmaMultiChan::onComplete)
      );

      /*
       * Enable the DMA interrupt
       */

      dma.enableInterrupts(Adc1DmaChannelInterruptFeature::COMPLETE);

      /**
       * start the DMA (i.e. make it ready to receive requests from the ADC peripheral)
       */

      dma.beginRead(readBuffer,2);

      /*
       * Go into an infinite loop converting
       */

      for(;;) {

        /*
         * For this test code we set the 4 words to a known pattern so we can tell if they
         * get properly overwritten by the DMA during conversion
         */
        readBuffer[0]=readBuffer[1]=0xAAAA;

        /*
         * Start a conversion and wait until the interrupt handler tells us
         * that it's finished.
         */
        adc.startRegularConversion();

        while(!_ready);
        _ready=false;
#ifdef DEBUG_PORT
        /*
         * write the values to the USART
         */
        debug << "Converted values are "
                     << StringUtil::Ascii(readBuffer[0]) << ", "
                     << StringUtil::Ascii(readBuffer[1]) << "\r\n";
#endif

        if( readBuffer[0] > 4000 ){
        	led.On();
        }
        else{
        	led.Off();
        }

        // wait for a second before converting the next set of values
        MillisecondTimer::delay(100);
      }
    }


    /**
     * Interrupt handler for the DMA complete event. Set the ready flag
     * when it's received.
     */

    void onComplete(DmaEventType det) {
      if(det==DmaEventType::EVENT_COMPLETE)
        _ready=true;
    }
};

int main() {

  Nvic::initialise();
  MillisecondTimer::initialise();

  AdcSingleDmaMultiChan adc;
  adc.run();

  // not reached
  return 0;
}
