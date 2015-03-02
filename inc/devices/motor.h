#pragma once

#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timer.h"

namespace stm32plus{
	template<class TimMotor, class ChannelA, class ChannelB>
	class Motor : TimMotor{
		public:
			enum{
				MAX_COMPARE=500
			};
			Motor():TimMotor(){
				this->setTimeBaseByFrequency(10000000, MAX_COMPARE-1);
				ChannelA::initCompareForPwmOutput();
				ChannelB::initCompareForPwmOutput();
				this->enablePeripheral();
			}
			void setOutput(float duty){
			  if(duty > 1.0f){
				  duty = 1.0f;
			  }
			  else if(duty < -1.0f){
				  duty = -1.0f;
			  }
			  duty = duty * 0.95;

			  int16_t value = (int16_t)(duty * MAX_COMPARE);
			  if(value>0){
			      ChannelA::setCompare(MAX_COMPARE);
			      ChannelB::setCompare(MAX_COMPARE-value);
			  } else {
			      ChannelA::setCompare(MAX_COMPARE+value);
			      ChannelB::setCompare(MAX_COMPARE);
			  }
			}
			inline void stop(){
				setOutput(0);
			}
	};
}
