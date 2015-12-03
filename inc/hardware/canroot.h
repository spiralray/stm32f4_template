#pragma once

#include "config/stm32plus.h"
#include "config/can.h"
#include <vector>

#include "utils/debug.h"

using namespace std;

#define CAN_BUFSIZE 64        //must be a number power of 2

namespace stm32plus {

  class CanNode{
  public:
    virtual void update(CanRxMsg *RxMessage);
  };

  typedef struct CAN_Buffer
  {
    /* @brief Receive buffer. */
    volatile CanRxMsg RX[CAN_BUFSIZE];
    /* @brief Receive buffer head. */
    volatile uint16_t RX_Head;
    /* @brief Receive buffer tail. */
    volatile uint16_t RX_Tail;
  } CAN_Buffer_t;

  template<class TCan>
  class CanRoot : public TCan {
  protected:
    vector<CanNode*> nodes;
    CAN_Buffer_t buffer;

  public:
    CanRoot(const typename TCan::Parameters& params) : TCan(params){
    	buffer.RX_Tail = 0;
    	buffer.RX_Head = 0;

    	TCan::CanInterruptEventSender.insertSubscriber(CanInterruptEventSourceSlot::bind(this,&CanRoot::onCanInterrupt));
    	TCan::enableInterrupts(CAN_IT_FMP0);
    }

    void AddListenerNode(CanNode* node){
      nodes.push_back( node );
    }

    void update(){
      CanRxMsg RxMessage;
      while( CAN_RXBufferData_Available() ){
	    RxMessage = CAN_RXBuffer_GetData();
	    for(unsigned int i=0; i< nodes.size() ; i++){
	        nodes[i]->update(&RxMessage);
	    }
      }
    }

    /*
     * receive IRQ callback
     */

    void onCanInterrupt(CanEventType cet) {

    	CanRxMsg msg;

    	/* Advance buffer head. */
    	uint16_t tempRX_Head = (buffer.RX_Head + 1) & (CAN_BUFSIZE-1);
    	/* Check for overflow. */
    	uint16_t tempRX_Tail = buffer.RX_Tail;

    	if(cet == CanEventType::EVENT_FIFO0_MESSAGE_PENDING) {
    		TCan::receive(CAN_FIFO0, &msg);

    		if (tempRX_Head == tempRX_Tail) {
    			/* Disable the CAN1 Receive interrupt */
    			//CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);
    		}else{
    			buffer.RX[(&buffer)->RX_Head] = msg;
    			buffer.RX_Head = tempRX_Head;
    		}
    	}
    }

    /**************************************************************************/
    /*!
        Check CAN RX Buffer Empty.
     */
    /**************************************************************************/
    uint8_t CAN_RXBufferData_Available()
    {
    	/* Make copies to make sure that volatile access is specified. */
    	uint16_t tempHead = buffer.RX_Head;
    	uint16_t tempTail = buffer.RX_Tail;

    	/* There are data left in the buffer unless Head and Tail are equal. */
    	return (tempHead != tempTail);
    }

    /**************************************************************************/
    /*!
        Get CanRxMsg with Buffering.
     */
    /**************************************************************************/
    CanRxMsg CAN_RXBuffer_GetData()
    {
    	CanRxMsg ans;

    	__disable_irq();

    	//ans = (buffer.RX[buffer.RX_Tail]);
    	memcpy(&ans, (const void*)&buffer.RX[buffer.RX_Tail], sizeof(CanRxMsg));

    	/* Advance buffer tail. */
    	buffer.RX_Tail = (buffer.RX_Tail + 1) & (CAN_BUFSIZE-1);

    	__enable_irq();

    	return ans;
    }
  };
}
