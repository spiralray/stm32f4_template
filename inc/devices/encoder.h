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
				position = 0;
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
			inline void updatePosition(){
			  captureSpeed();
			  position += getSpeed();
			}
			inline void setPosition(int32_t _position) {
			  uint16_t nowCounter=TimerEnc::getCounter();
			  prevCounter=nowCounter;
			  position = _position;
			}
			int32_t getPosition() const{
			  return position;
			}
		private:
			uint16_t prevCounter;
			int16_t speed;
			int32_t position;
	};
}
