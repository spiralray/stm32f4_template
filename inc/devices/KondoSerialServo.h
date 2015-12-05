/*
 * KondoSerialServo.h
 *
 *  Created on: 2015/12/03
 *      Author: spiralray
 */

#include "config/stm32plus.h"
#include "config/usart.h"

/*
 * sample
  KondoSerialServo<
  Usart3<>,
  Usart3RxDmaChannel<UsartDmaReaderFeature<Usart3PeripheralTraits> >
  > servo;
*/

template<class USARTX, class UsartXRxDmaChannel>
class KondoSerialServo {

public:

	USARTX *usart;

	UsartXRxDmaChannel *dmaReader;
	uint8_t rxBuff[6];


	KondoSerialServo(){
		typename USARTX::Parameters params(115200);
		params.usart_wordLength=USART_WordLength_9b;
		params.usart_parity=USART_Parity_Even;
		params.usart_stopBits=USART_StopBits_1;

		usart = new USARTX(params);
		dmaReader = new UsartXRxDmaChannel();
	}

	uint16_t send(uint8_t id, uint16_t angle) {
		uint8_t txBuff[3] = {0x80|id , (angle>>7) & 0x7f , angle & 0x7f };

		dmaReader->beginRead(rxBuff,6);

		for(int i=0;i<3;i++){
			usart->send(txBuff[i]);
		}

		dmaReader->waitUntilComplete();

		return (uint16_t)(rxBuff[4]<<7 | rxBuff[5]);

	}
};

