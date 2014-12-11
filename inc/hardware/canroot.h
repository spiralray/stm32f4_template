#pragma once

#include "config/stm32plus.h"

#include <vector>
#include "hardware/can.h"

using namespace std;

namespace stm32plus {

  class CanNode{
  public:
    virtual void Update(CanRxMsg *RxMessage);
  };

  class CanRoot{
  protected:
    vector<CanNode*> nodes;

  public:
    CanRoot(){
      CAN1_Configuration();
    }

    void AddListenerNode(CanNode* node){
      nodes.push_back( node );
    }

    void Update(){
      CanRxMsg RxMessage;
      while( CAN_RXBufferData_Available(&CAN1_Buf) ){
	  RxMessage = CAN_RXBuffer_GetData(&CAN1_Buf);
	  for(unsigned int i=0; i< nodes.size() ; i++){
	      nodes[i]->Update(&RxMessage);
	  }
      }
    }

    void Send(CanTxMsg *TxMessage){
      uint8_t TransmitMailbox1 = CAN_Transmit(CAN1, TxMessage);
      while(CAN_TransmitStatus(CAN1, TransmitMailbox1) != CANTXOK);
    }
  };
}
