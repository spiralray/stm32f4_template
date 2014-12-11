#pragma once

#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timer.h"

#include "../utils/mymath.h"

namespace stm32plus{
	template<class TimerEnc>
	class Encoder : public TimerEnc {
		public:
			Encoder():TimerEnc(),prevCounter(0){
				TimerEnc::initialiseEncoderCounter(65535);

				TimerEnc::setCounter(0);
				TimerEnc::enablePeripheral();

				speed= 0;
				location = 0;
			}
			~Encoder(){}
			inline void captureSpeed(){
				uint16_t nowCounter=TimerEnc::getCounter();
				int32_t diff=nowCounter-prevCounter;
				prevCounter=nowCounter;
				if(abs(diff)>32767){
					speed=diff-MyMath::signof(diff)*65536;
				} else {
					speed=diff;
				}
			}
			int16_t getSpeed() const{
				return speed;
			}
			inline void updateLocation(){
			  captureSpeed();
			  location += getSpeed();
			}
			inline void setLocation(int32_t _location) {
			  uint16_t nowCounter=TimerEnc::getCounter();
			  prevCounter=nowCounter;
			  location = _location;
			}
			int32_t getLocation() const{
			  return location;
			}
		private:
			uint16_t prevCounter;
			int16_t speed;
			int32_t location;
	};
}
