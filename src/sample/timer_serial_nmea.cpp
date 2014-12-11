/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"

#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"
#include "utils/debug.h"

#include "board/main_v3.h"

#include "utils/nmea0183.h"


using namespace stm32plus;

/* Defines -------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/
MainV3 *mainBoard;
/* Constants -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

class OutputTimer {
public:
  OutputTimer(){
    timer.setTimeBaseByFrequency(1000, 20);
    // 割り込みハンドラをバインド
    timer.TimerInterruptEventSender.insertSubscriber(
	TimerInterruptEventSourceSlot::bind(this, &OutputTimer::onInterrupt)
    );
    timer.enableInterrupts(TIM_IT_Update); // オーバーフロー割り込み有効化
    timer.enablePeripheral();
  }

  // 割り込みハンドラ
  void onInterrupt(TimerEventType tet,uint8_t timerNumber){
    // オーバーフロー割り込みの場合
    if(tet==TimerEventType::EVENT_UPDATE){
	char string[256];
	char data[256];

	sprintf(data, "M3ENC,%d,%d,%d,%d", mainBoard->encoders.getCounter1(), mainBoard->encoders.getCounter2(), mainBoard->encoders.getCounter3(), mainBoard->encoders.getCounter4() );
	uint8_t checksum = NMEA0183::calcCheckSum(data);
	sprintf(string,"$%s*%02x\r\n", data, checksum );
	//debug << string;
	mainBoard->com.puts(string);

    }
  }
private:
  int i=0;
  Timer6<Timer6InternalClockFeature,Timer6InterruptFeature> timer;
};

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

  mainBoard = new MainV3();
  OutputTimer outputtimer;

  while(1);

}

/* End Of File ---------------------------------------------------------------*/

