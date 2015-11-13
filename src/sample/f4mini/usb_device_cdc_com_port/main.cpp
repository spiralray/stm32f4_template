/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usb/device/cdc.h"


using namespace stm32plus;
using namespace stm32plus::usb;


/**
 * This example demonstrates the USB Communications Device Class (CDC) in the form of
 * a virtual COM port. Virtual COM ports allow you to use software on your PC that was designed
 * to talk to a serial port to communicate over USB to your STM32.
 *
 * To work as a USB device, connect PA9 to VBUS
 */

class UsbDeviceCdcComPort {

protected:

	enum{
		BUFSIZE = 2048
	};
	/* @brief Receive buffer. */
	volatile uint8_t RX[BUFSIZE];
	/* @brief Receive buffer head. */
	volatile uint16_t RX_Head;
	/* @brief Receive buffer tail. */
	volatile uint16_t RX_Tail;

public:

	/*
	 * declare a type for the the USB stack
	 */

	typedef ComPortCdcDevice<
			InternalFsPhy<>,                    // use the internal full speed PHY with no features
			ManufacturerTextFeature,            // we'll supply a manufacturer name
			ProductTextFeature,                 // ... and a product name
			SerialNumberTextFeature,            // ... and a serial number
			ConfigurationTextFeature            // ... and a config text string
			> MyUsb;

	MyUsb usb;

	/*
	 * The current line coding (baud rate, parity etc)
	 * The CdcLineCoding constructor sets a default of 9600/8/N/1
	 */

	CdcLineCoding _lineCoding;

	UsbDeviceCdcComPort(): usb(){
		RX_Tail = 0;
		RX_Head = 0;

		// initialise

		/*
		 * set up the parameters for the USB CDC device. Do not attempt to reuse vid/pid combinations unless
		 * you know how to flush your PC's USB driver cache because Windows caches the characteristics of each
		 * device and will suspend your device if it suddenly re-appears as a different device type.
		 */

		MyUsb::Parameters usbParams;

		usbParams.device_vid=0xDEAD;           // demo vendor ID
		usbParams.device_pid=0x0010;           // demo product ID

		usbParams.device_manufacturer_text="Andy's Workshop";   // see params.device_language_[ids/count] to change the languages
		usbParams.device_product_text="stm32plus virtual COM port";
		usbParams.device_serial_text="0123456789";
		usbParams.device_configuration_text="My configuration";

		usbParams.cdc_com_port_rx_buffer_size=16;  // default of 1Kb is far too big for this app

		/*
		 * Subscribe to errors
		 */

		usb.UsbErrorEventSender.insertSubscriber(UsbErrorEventSourceSlot::bind(this,&UsbDeviceCdcComPort::onError));

		/*
		 * Subscribe to USB events - data received from the host will be notified to us
		 * asynchronously.
		 */

		usb.UsbEventSender.insertSubscriber(UsbEventSourceSlot::bind(this,&UsbDeviceCdcComPort::onEvent));

		/*
		 * Start the USB peripheral. It will run asynchronously. There is no requirement
		 * for the parameters to remain in scope after the initialise call
		 */

		if(!usb.initialise(usbParams))
			for(;;);      // onError() has already locked up
	}

	bool send(uint8_t data)
	{
		usb.transmit(&data,1);
	}

	void send(char *str){
		usb.transmit(str,strlen(str));
	}

	bool dataAvailable(){
		uint16_t tempHead = RX_Head;
		uint16_t tempTail = RX_Tail;
		/* There are data left in the buffer unless Head and Tail are equal. */
		return (tempHead != tempTail);
	}

	uint8_t receive(){
		uint8_t ans;

		__disable_irq();
		ans = (RX[RX_Tail]);
		/* Advance buffer tail. */
		RX_Tail = (RX_Tail + 1) & (BUFSIZE-1);
		__enable_irq();

		return ans;
	}

	/**
	 * Event callback from the USB stack. Lots of stuff will come through here but
	 * we're only interested in data arriving from the host and control messages
	 */

	void onEvent(UsbEventDescriptor& ued) {

		// reject all events that we don't care about

		if(ued.eventType==UsbEventDescriptor::EventType::CDC_DATA_RECEIVED)
			onData(static_cast<CdcDataReceivedEvent&>(ued));
		else if(ued.eventType==UsbEventDescriptor::EventType::CDC_CONTROL)
			onControl(static_cast<CdcControlEvent&>(ued));
	}


	/**
	 * Control event received from the host
	 */

	void onControl(CdcControlEvent& event) {

		switch(event.opcode) {

		/*
		 * We don't process these. They're enumerated here so you can
		 * see what's available.
		 */

		case CdcControlCommand::SEND_ENCAPSULATED_COMMAND:
		case CdcControlCommand::GET_ENCAPSULATED_RESPONSE:
		case CdcControlCommand::SET_COMM_FEATURE:
		case CdcControlCommand::GET_COMM_FEATURE:
		case CdcControlCommand::CLEAR_COMM_FEATURE:
		case CdcControlCommand::SET_CONTROL_LINE_STATE:
		case CdcControlCommand::SEND_BREAK:
			break;

		case CdcControlCommand::SET_LINE_CODING:      // set a new line encoding
			memcpy(&_lineCoding,event.data,sizeof(_lineCoding));
			break;

		case CdcControlCommand::GET_LINE_CODING:      // return the current line encoding
			event.data=reinterpret_cast<uint8_t *>(&_lineCoding);
			break;

		default:
			break;
		}
	}


	/**
	 * Data received from the host
	 */

	void onData(CdcDataReceivedEvent& event) {

		// add character to the message to send

		for(int i=0;i<event.size;i++){

			/* Advance buffer head. */
			uint16_t tempRX_Head = (RX_Head + 1) & (BUFSIZE-1);

			/* Check for overflow. */
			uint16_t tempRX_Tail = RX_Tail;
			uint8_t data = event.data[i];

			if (tempRX_Head == tempRX_Tail) {
				/* Disable the Receive interrupt */

			}else{
				RX[RX_Head] = data;
				RX_Head = tempRX_Head;
			}
		}

		usb.beginReceive();
	}


	/**
	 * USB error event received
	 * @param uee the event descriptor
	 */

	void onError(UsbErrorEvent& uee) {

		// ignore unconfigured errors from the CDC device

		if(uee.provider==ErrorProvider::ERROR_PROVIDER_USB_DEVICE && uee.code==MyUsb::E_UNCONFIGURED)
			return;

		// flash the RED led on PD2 at 1Hz

		GpioD<DefaultDigitalOutputFeature<2>> pd;

		for(;;) {
			pd[2].reset();
			MillisecondTimer::delay(500);
			pd[2].set();
			MillisecondTimer::delay(500);
		}
	}
};


/*
 * Main entry point
 */

int main() {

	// initialise the interrupt controller

	Nvic::initialise();

	// initialise the millisecond timer

	MillisecondTimer::initialise();

	// run the test

	UsbDeviceCdcComPort com;

	for(;;){
		if( com.dataAvailable() ){
			com.send(com.receive());
		}
	}

	// not reached
	return 0;
}
