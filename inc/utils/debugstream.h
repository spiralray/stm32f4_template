#pragma once

#include "config/stm32plus.h"
#include "config/usart.h"

using namespace stm32plus;

template<class T> 
class DebugStream : public TextOutputStream{
public:
	DebugStream(uint32_t baud):TextOutputStream(usartos),usart(baud),usartos(usart){
	}
private:
	T usart;
	UsartPollingOutputStream usartos;
};
