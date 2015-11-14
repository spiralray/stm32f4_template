#pragma once

#include "config/stm32plus.h"
#include "config/usart.h"
#include "utils/UsartInterruptWithBuffer.h"
#include "utils/UsartInterruptOutputStream.h"
#include "hardware/UsbDeviceCdcComPort.h"

using namespace stm32plus;

template<class T>
class DebugStream : public TextOutputStream{
public:
	DebugStream(uint32_t baud):TextOutputStream(usartos),usart(baud),usartos(usart){
	}

public:
	T usart;
	UsartInterruptOutputStream<T> usartos;
};
