#pragma once

#include "config/stm32plus.h"
#include "hardware/canroot.h"

#define PS_CHECK(but)   (getButtonPress(but))

#define UP              0x00
#define RIGHT           0x01
#define DOWN            0x02
#define LEFT            0x03
#define TRIANGLE        0x04
#define CIRCLE          0x05
#define CROSS           0x06
#define SQUARE          0x07

#define L1              0x08
#define R1              0x09
#define L2              0x0A
#define R2              0x0B
#define SELECT          0x0C
#define START           0x0D
#define PS              0x0E
#define CONNECTED       0x0F

enum{
  ANALOG_L_X = 0,
  ANALOG_L_Y,
  ANALOG_R_X,
  ANALOG_R_Y,
  ANALOG_L1,
  ANALOG_R1,
  ANALOG_L2,
  ANALOG_R2,
  ANALOG_UP,
  ANALOG_RIGHT,
  ANALOG_DOWN,
  ANALOG_LEFT
};

namespace stm32plus {
  class PS3Con : public CanNode{
  protected:
    CanRxMsg data[2];

  public:
    PS3Con(){
      for(int i=0;i<8;i++){
	  data[0].Data[i] = 0x00;
	  data[1].Data[i] = 0x00;
      }
    }

    void Update(CanRxMsg *RxMessage){

      if( RxMessage->StdId == 0x321 && RxMessage->DLC == 8 ){
	  data[0]= *RxMessage;
      }
      else if( RxMessage->StdId == 0x322 && RxMessage->DLC == 6 ){
	  data[1]= *RxMessage;
      }

    }

    int getButtonPress(int buttonID){
      if( buttonID < 8 ){
          return data[1].Data[0] & (0x01<<buttonID);
      }
      else{
          return data[1].Data[1] & (0x01<<(buttonID-8));
      }
    }

    int getAnalog(int buttonID){
      if( buttonID < 8 ){
	  return data[0].Data[buttonID];
      }
      else{
	  return data[0].Data[buttonID-8+2];
      }
    }

    static int analog_point(int ps)
    {
    	if(ps < 128){
    		ps = (int)(127 - ps);
    	}
    	else if(ps > 127){
    		ps = (int)(ps - 128);
    	}
    	else{
    		ps = (int)0;
    		}
    	return(ps);
    }

    static int analog_ded_100(int analog_point)
    {

    	if(analog_point <= 100){
    		analog_point=(char)(100 - analog_point);
    	} else if(analog_point>= 155){
    		analog_point=(char)(155 - analog_point);
    	} else{
    		analog_point= 0;
    	}
    	return analog_point;
    }

  };
}
