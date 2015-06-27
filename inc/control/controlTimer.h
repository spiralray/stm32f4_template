#pragma once

template<class TTimer, class TMachine>
class ControlTimer : public TTimer {
	public:
		ControlTimer(TMachine* _machine, uint16_t freq, uint8_t _priority=0):machine(_machine),priority(_priority){
			this->setTimeBaseByFrequency(60000, 60000/freq - 1); // 1000Hz: 1ms
		}
		~ControlTimer(){}
		void bind(void (TMachine::*callback)(TimerEventType,uint8_t)){
			this->disablePeripheral();
			this->TimerInterruptEventSender.insertSubscriber(
					TimerInterruptEventSourceSlot::bind(machine, callback)
					);
			this->setNvicPriorities(priority);
			this->enableInterrupts(TIM_IT_Update);
			this->enablePeripheral();
		}
	private:
		TMachine *machine;
		uint8_t priority;
};
