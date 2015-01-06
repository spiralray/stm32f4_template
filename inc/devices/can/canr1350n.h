#pragma once

#include "config/stm32plus.h"
#include "hardware/canroot.h"

namespace stm32plus {
  class CanR1350N : public CanNode{
  protected:
    CanRxMsg data;

  public:
    CanR1350N(){
      for(int i=0;i<8;i++){
	  data.Data[i] = 0x00;
      }
    }

    void Update(CanRxMsg *RxMessage){
    	if( RxMessage->StdId == 0x220 && RxMessage->DLC == 2 ){
    		data= *RxMessage;
    	}
    }

    int16_t get(){
    	return *(int16_t *)(data.Data);
    }

  };
}
