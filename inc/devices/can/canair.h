/*
 * led.h
 *
 *  Created on: 2014/10/05
 *      Author: spiralray
 */

#pragma once

#include "config/stm32plus.h"
#include "hardware/canroot.h"

namespace stm32plus{

  class CanAir {
  private:
    CanTxMsg TxMessage;
    Can *can;
  public:
    CanAir(Can *_can){
      can = _can;

      TxMessage.StdId = 0x150;
      TxMessage.ExtId = 0x01;
      TxMessage.RTR = CAN_RTR_DATA;
      TxMessage.IDE = CAN_ID_STD;
      TxMessage.DLC = 1;
      TxMessage.Data[0] = 0x00;
    }
    CanAir(Can *_can, uint32_t stdid){
      can = _can;

      TxMessage.StdId = stdid;
      TxMessage.ExtId = 0x01;
      TxMessage.RTR = CAN_RTR_DATA;
      TxMessage.IDE = CAN_ID_STD;
      TxMessage.DLC = 1;
      TxMessage.Data[0] = 0x00;
    }

    void Set(int ch){
      TxMessage.Data[0] |= 0x01 << ch;
    }

    void Reset(int ch){
      TxMessage.Data[0] &= ~(0x01 << ch);
    }

    void Update(){
      can->send(TxMessage);
    }

  };

}
