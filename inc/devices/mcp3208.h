#pragma once

#include "config/stm32plus.h"
#include "utils/debug.h"

#include "config/gpio.h"
#include "config/spi.h"

namespace stm32plus{

  class MCP3208{
  protected:
    Spi& _spi;
    int ch;
    float source_voltage;

  public:
    MCP3208(Spi& spi, int _ch): _spi(spi){
      ch = _ch;
      source_voltage = 5.0f;
    }

    MCP3208(Spi& spi, int _ch, float _source_voltage): _spi(spi){
      ch = _ch;
      source_voltage = _source_voltage;
    }

    uint16_t get(){
      uint8_t data[3] = { 0x00 };
      uint8_t ad[2];

      data[0] = 0x06+(ch>>2);
      data[1] = 0xff & ch<<6;
      data[2] = 0x00;

      _spi.setNss(false);
      while(!_spi.readyToSend());
      _spi.send(&(data[0]),1);
      while(!_spi.readyToSend());
      _spi.send(&(data[1]),1, ( &ad[0]) );
      while(!_spi.readyToSend());
      _spi.send(&(data[2]),1, ( &ad[1]) );
      _spi.setNss(true);

      return ((ad[0]&0x0f))*256+ad[1];
    }

    float getVoltage(){
      return source_voltage * ((float)( get() ) /4095);
    }

  };

}
