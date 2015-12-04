/*
 * canloop.cpp
 *
 *  Created on: 2015/02/02
 *      Author: Kentaro Tanaka
 */




/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"

#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"
#include "utils/debug.h"

#include "board/f4mini.h"
#include "hardware/canroot.h"

#include "utils/debug.h"

using namespace stm32plus;

/* Defines -------------------------------------------------------------------*/

class CanLoop : public CanNode{
protected:
	Can *can;
public:
	CanLoop( Can *_can ){
		can = _can;
	}

	void update(CanRxMsg *RxMessage){
		printf("CanLoop StdId: 0x%03x Data: ",RxMessage->StdId);
		for(int i=0; i<RxMessage->DLC; i++){
			printf("%02x ",RxMessage->Data[i]);
		}
		printf("\r\n");
		do{
			errorProvider.clear();
			can->send( *(CanTxMsg*)RxMessage );
		}while( errorProvider.getCode() == Can::E_TX_NO_MAILBOX );
	}

};

class CanListen : public CanNode{
public:
	CanListen( ){
	}

	void update(CanRxMsg *RxMessage){
		printf("CanListen StdId: 0x%03x Data: ",RxMessage->StdId);
		for(int i=0; i<RxMessage->DLC; i++){
			printf("%02x ",RxMessage->Data[i]);
		}
		printf("\r\n");
	}

};

/* Variables -----------------------------------------------------------------*/

/* Constants -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/**************************************************************************/
/*!
    @brief  Main Program.
    @param  None.
    @retval None.
 */
/**************************************************************************/
int main(void)
{
	uint32_t t1,t2,prescaler;

	//Initialise Systick
	MillisecondTimer::initialise();

	Nvic::initialise();

	F4Mini board;

	RCC_ClocksTypeDef Clocks;
	RCC_GetClocksFreq(&Clocks);

	//-------------------------------------------------------------------------------------------------Initialize CAN1
	CanRoot< Can1_Remap1< Can1InterruptFeature, Can1FilterBypassFeature> > can1( { 500000,500 } );

	t1 = ((CAN1->BTR >> 16)& 0x0f) +1;
	t2 = ((CAN1->BTR >> 20)& 0x0f) +1;
	prescaler = (CAN1->BTR & 0xffff) +1;

	debug << "=====CAN1=====\r\n";
	debug << "Clocks.PCLK1_Frequency " << Clocks.PCLK1_Frequency << "\r\n";
	debug << "t1 " << t1 << "\r\n";
	debug << "t2 " << t2 << "\r\n";
	debug << "prescaler " << prescaler << "\r\n";
	debug << "baudrate " << Clocks.PCLK1_Frequency / ( (t1+t2+1)*prescaler) /1000<< "kbps\r\n";

	CanLoop *loop = new CanLoop(&can1);
	can1.AddListenerNode(loop);

	//-------------------------------------------------------------------------------------------------Initialize CAN2
	CanRoot< Can2_Remap< Can2InterruptFeature, Can2FilterBypassFeature> > can2( { 500000,500 } );

	t1 = ((CAN2->BTR >> 16)& 0x0f) +1;
	t2 = ((CAN2->BTR >> 20)& 0x0f) +1;
	prescaler = (CAN2->BTR & 0xffff) +1;

	debug << "=====CAN2=====\r\n";
	debug << "Clocks.PCLK1_Frequency " << Clocks.PCLK1_Frequency << "\r\n";
	debug << "t1 " << t1 << "\r\n";
	debug << "t2 " << t2 << "\r\n";
	debug << "prescaler " << prescaler << "\r\n";
	debug << "baudrate " << Clocks.PCLK1_Frequency / ( (t1+t2+1)*prescaler) /1000<< "kbps\r\n";

	CanListen *listen = new CanListen();
	can2.AddListenerNode(listen);

	debug << "==============\r\n";

	//-------------------------------------------------------------------------------------------------Communicate

	while(1){
		debug << "send\r\n";

		do{
			errorProvider.clear();
			can2.send(0x100, 7, "test\r\n");
		}while( errorProvider.getCode() == Can::E_TX_NO_MAILBOX );

		for( int i=0; i<1000 ; i++){
			can1.update();
			can2.update();
			MillisecondTimer::delay(1);
		}

		board.led.Toggle();
	}

	return 0;

}

/* End Of File ---------------------------------------------------------------*/
