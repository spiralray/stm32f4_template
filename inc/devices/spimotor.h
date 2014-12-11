#pragma once

#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/spi.h"

namespace stm32plus{

	class SpiMotor{
	protected:
	  Spi& _spi;
	  int ch;
	public:
	  enum{
	    MAX_COMPARE=500
	  };
	  SpiMotor(Spi& spi, int _ch): _spi(spi){
	    ch = _ch;
	  }
	  void setOutput(float duty){
	    int16_t value = (int16_t)(duty * MAX_COMPARE);
	    uint8_t invert = 0;
	    uint8_t data[2] = { 0 };


	    if(value>MAX_COMPARE) value=MAX_COMPARE;
	    if(value<-MAX_COMPARE) value=-MAX_COMPARE;

	    if( value < 0 ){
		value = 0-value;
		invert = 1;
	    }
	    data[0] = ( ch << 5 ) | (invert << 4) | ((value>>5)&0x0f);
	    data[1] = 0x80| (value&0x1f);

	    _spi.setNss(false);
	    while(!_spi.readyToSend());
	    _spi.send(&data[0],1);
	    while(!_spi.readyToSend());
	    _spi.send(&data[1],1);
	    _spi.setNss(true);
	  }
	  inline void stop(){
	    setOutput(0);
	  }

	};
}
