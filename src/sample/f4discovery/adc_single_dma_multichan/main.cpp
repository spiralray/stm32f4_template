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

#include "utils/debug.h"


using namespace stm32plus;

//DMA buffer must be static to avoid assigning to CCM
volatile uint16_t buffer[4];


class AdcSingleDmaMultiChan {

private:
	volatile bool _ready;
	uint16_t *readBuffer;

public:

	AdcSingleDmaMultiChan(uint16_t *_buffer){
		readBuffer = _buffer;
	}

	void run() {

		_ready=false;

		/*
		 * Declare the ADC1 DMA channel. The default is circular mode for the AdcDmaFeature
		 * which means that it wil automatically refill our buffer on each conversion because
		 * one conversion exactly matches the size of the memory buffer that we will give
		 * to the DMA peripheral.
		 */

		Adc1DmaChannel<AdcDmaFeature<Adc1PeripheralTraits>,Adc1DmaChannelInterruptFeature> dma;

		/*
		 * Declare the ADC peripheral with an APB2 clock prescaler of 2, a resolution of
		 * 12 bits. We will use 144-cycle conversions on ADC channels 0,1 and a 480-cycle
		 * conversion on ADC channel 2. Scan mode is used with the default template parameter
		 * that causes EOC to be raised at the end of a complete conversion group.
		 */

		Adc1<
		AdcClockPrescalerFeature<2>,                // prescaler of 2
		AdcResolutionFeature<12>,                   // 12 bit resolution
		Adc1Cycle144RegularChannelFeature<0,1>,     // using channels 0,1 on ADC1 with 144-cycle latency
		Adc1Cycle480RegularChannelFeature<2>,       // using channel 2 on ADC1 with 480-cycle latency
		Adc1Cycle480TemperatureSensorFeature,       // using the temperature sensor channel
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

		printf("Data buffer address is 0x%08x\r\n", (uint32_t)readBuffer);

		if( (uint32_t)readBuffer >= 0x10000000 && (uint32_t)readBuffer < 0x10000000 + 64*1024 ){
			debug << "ERROR! Data buffer is on Core-Coupled SRAM! DMA cannot be used on CCM.\r\n";
		}

		/**
		 * start the DMA (i.e. make it ready to receive requests from the ADC peripheral)
		 */

		dma.beginRead(readBuffer,4);

		/*
		 * Go into an infinite loop converting
		 */

		for(;;) {

			/*
			 * For this test code we set the 4 words to a known pattern so we can tell if they
			 * get properly overwritten by the DMA during conversion
			 */

			readBuffer[0]=readBuffer[1]=readBuffer[2]=readBuffer[3]=0xAAAA;

			/*
			 * Start a conversion and wait until the interrupt handler tells us
			 * that it's finished.
			 */

			adc.startRegularConversion();

			while(!_ready);
			_ready=false;

			/*
			 * write the values to the USART
			 */

			debug << "Converted values are "
					<< StringUtil::Ascii(readBuffer[0]) << ", "
					<< StringUtil::Ascii(readBuffer[1]) << ", "
					<< StringUtil::Ascii(readBuffer[2]) << ", temperature="
					<< StringUtil::Ascii(adc.getTemperature(readBuffer[3])) << "\r\n";

			// wait for a second before converting the next set of values

			MillisecondTimer::delay(1000);
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

	// we're using interrupts, initialise NVIC

	Nvic::initialise();

	MillisecondTimer::initialise();

	AdcSingleDmaMultiChan adc(buffer);
	adc.run();

	// not reached
	return 0;
}
