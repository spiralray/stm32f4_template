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

#include "board/main_v3.h"
#include "hardware/canroot.h"

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
    can->send( *(CanTxMsg*)RxMessage );
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

  //Initialise Systick
  MillisecondTimer::initialise();

  Nvic::initialise();

  MainV3 mainBoard;

  MillisecondTimer::delay(100);

  CanLoop *loop = new CanLoop(&mainBoard.can);
  mainBoard.can.AddListenerNode(loop);

  while(1){
      mainBoard.can.update();
  }

  while(1);

}

/* End Of File ---------------------------------------------------------------*/
