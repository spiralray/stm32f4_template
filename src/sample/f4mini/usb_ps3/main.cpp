//To use USB Host Library, set USE_USB = USE_USBHOST in makefile

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"

#include "devices/led.h"

#include "usb_bsp.h"
#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_hid_core.h"
#include "usbh_hid_ps3.h"

using namespace stm32plus;

/* Defines -------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/

USB_OTG_CORE_HANDLE           USB_OTG_Core_dev;
USBH_HOST                     USB_Host;

/* Constants -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

//Process USB Host every 5ms
class USBTimer {
public:
  USBTimer(){
    timer.setTimeBaseByFrequency(1000, 5-1);
    timer.TimerInterruptEventSender.insertSubscriber(
	TimerInterruptEventSourceSlot::bind(this, &USBTimer::onInterrupt)
    );
    timer.setNvicPriorities(2);
    timer.enableInterrupts(TIM_IT_Update);
    timer.enablePeripheral();
  }

  void onInterrupt(TimerEventType tet,uint8_t timerNumber){
    if(tet==TimerEventType::EVENT_UPDATE){
    	/* Host Task handler */
    	USBH_Process(&USB_OTG_Core_dev , &USB_Host);
    }
  }
private:
  Timer6<Timer6InternalClockFeature,Timer6InterruptFeature> timer;
};

/**************************************************************************/
/*
    @brief  Main Program.
	@param  None.
    @retval None.
 */
/**************************************************************************/
int main(void)
{
  //Initialise Systick
  MillisecondTimer::initialise();

  GpioD<DefaultDigitalOutputFeature<2> > pd;
  Led led(pd[2]);

  /* Init Host Library */
  USBH_Init(&USB_OTG_Core_dev,
		  USB_OTG_FS_CORE_ID,
		  &USB_Host,
		  &HID_cb,
		  &USR_Callbacks);


  USBTimer timer;

  while(1){

	  if( ps3_connected ){

		  //PS button
		  if( HID_PS3_Data.button[2] & 0x01 )	led.On();
		  else									led.Off();

#ifdef DEBUG_PORT
		  printf("Button state 0x%02x 0x%02x 0x%02x \n", HID_PS3_Data.button[0], HID_PS3_Data.button[1], HID_PS3_Data.button[2] );
		  printf("Analog state 0x%02x 0x%02x 0x%02x 0x%02x \n", HID_PS3_Data.analog_stick[0], HID_PS3_Data.analog_stick[1], HID_PS3_Data.analog_stick[2], HID_PS3_Data.analog_stick[3] );

		  printf("Analog state ");
		  for( int i=0 ; i<12 ; i++ ) printf("%02x ", HID_PS3_Data.button_analog[i]);
		  printf("\n");

		  printf("\n");
#endif
	  }

	  MillisecondTimer::delay(30);
  }

}

/* End Of File ---------------------------------------------------------------*/

