#pragma once

#include "config/stm32plus.h"
#include "utils/debug.h"

#include "config/gpio.h"
#include "config/spi.h"

#include "../utils/mymath.h"

namespace stm32plus{

  class SpiEncoder{
  protected:
    Spi& _spi;
    int ch;

  public:
    SpiEncoder(Spi& spi, int _ch): _spi(spi){
      ch = _ch;
      prevCounter = getCounter();
    }

    uint16_t getCounter(){
      uint8_t data[3] = { 0x00 };
      uint16_t enc;

      data[0] = 0x10+ch*2;
      data[1] = data[0]+1;
      data[2] = 0x00;

      _spi.setNss(false);
      while(!_spi.readyToSend());
      _spi.send(&(data[0]),1);
      while(!_spi.readyToSend());
      _spi.send(&(data[1]),1, ((uint8_t *)&enc) );
      while(!_spi.readyToSend());
      _spi.send(&(data[2]),1, ((uint8_t *)&enc)+1 );
      _spi.setNss(true);

      return enc ;
    }

    inline void captureSpeed(){
      uint16_t nowCounter=getCounter();
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
  private:
    uint16_t prevCounter;
    int16_t speed;
  };
}
