/**
 ******************************************************************************
 * @file    usbh_hid_mouse.c
 * @author  MCD Application Team
 * @version V2.1.0
 * @date    19-March-2012
 * @brief   This file is the application layer for USB Host HID Mouse Handling.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usbh_hid_ps3.h"

extern USB_OTG_CORE_HANDLE           USB_OTG_Core_dev;
extern USBH_HOST                     USB_Host;
/** @addtogroup USBH_LIB
 * @{
 */

/** @addtogroup USBH_CLASS
 * @{
 */

/** @addtogroup USBH_HID_CLASS
 * @{
 */

/** @defgroup USBH_HID_PS3
 * @brief    This file includes HID Layer Handlers for USB Host HID class.
 * @{
 */

/** @defgroup USBH_HID_PS3_Private_TypesDefinitions
 * @{
 */
/**
 * @}
 */


/** @defgroup USBH_HID_PS3_Private_Defines
 * @{
 */
/**
 * @}
 */


/** @defgroup USBH_HID_PS3_Private_Macros
 * @{
 */
/**
 * @}
 */

/** @defgroup USBH_HID_PS3_Private_FunctionPrototypes
 * @{
 */
static void  PS3_Init (void);
static void  PS3_Decode(uint8_t *data);
/**
 * @}
 */


/** @defgroup USBH_HID_PS3_Private_Variables
 * @{
 */
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined   (__CC_ARM) /*!< ARM Compiler */
__align(4)
#elif defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#elif defined (__GNUC__) /*!< GNU Compiler */
#pragma pack(4)
#elif defined  (__TASKING__) /*!< TASKING Compiler */
__align(4)
#endif /* __CC_ARM */
#endif


HID_PS3_Data_TypeDef HID_PS3_Data;
HID_cb_TypeDef HID_PS3_cb =
{
		PS3_Init,
		PS3_Decode,
};

const uint8_t PS3_REPORT_BUFFER[] = {
		0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00,
		0xff, 0x27, 0x10, 0x00, 0x32,
		0xff, 0x27, 0x10, 0x00, 0x32,
		0xff, 0x27, 0x10, 0x00, 0x32,
		0xff, 0x27, 0x10, 0x00, 0x32,
		0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/** Used to set the LEDs on the controllers */
const uint8_t LEDS[] = {
        0x01, // LED1
        0x02, // LED2
        0x04, // LED3
        0x08, // LED4

        0x09, // LED5
        0x0A, // LED6
        0x0C, // LED7
        0x0D, // LED8
        0x0E, // LED9
        0x0F // LED10
};

uint8_t writeBuf[EP_MAXPKTSIZE];
/**
 * @}
 */

USBH_Status PS3_Command(uint8_t* data, uint16_t nbytes){
	USB_Host.Control.setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_CLASS;
	USB_Host.Control.setup.b.bRequest = USB_HID_SET_REPORT;

	USB_Host.Control.setup.b.wValue.bw.msb = 0x01;
	USB_Host.Control.setup.b.wValue.bw.lsb = 0x02;
	USB_Host.Control.setup.b.wIndex.w = 0x00;
	USB_Host.Control.setup.b.wLength.w = nbytes;

	return USBH_CtlReq(&USB_OTG_Core_dev, &USB_Host, data , nbytes );

}

void setLedOn(int a) {
        writeBuf[9] |= (uint8_t)((LEDS[a] & 0x0f) << 1);
        PS3_Command(writeBuf, PS3_REPORT_BUFFER_SIZE);
}

/**
 * @brief  PS3_Init
 *         Init Mouse State.
 * @param  None
 * @retval None
 */
static void  PS3_Init (void)
{
	for (uint8_t i = 0; i < PS3_REPORT_BUFFER_SIZE; i++)
		writeBuf[i] = PS3_REPORT_BUFFER[i];

	for( int i=0 ; i<3 ; i++ )	HID_PS3_Data.button[i] = 0x00;
	for( int i=0 ; i<4 ; i++ )	HID_PS3_Data.analog_stick[i] = 0x80;
	for( int i=0 ; i<12 ; i++ )	HID_PS3_Data.button_analog[i] = 0x00;

	/* Call User Init*/
	USR_PS3_Init();
}

/**
 * @brief  PS3_Decode
 *         Decode Mouse data
 * @param  data : Pointer to Mouse HID data buffer
 * @retval None
 */
static void  PS3_Decode(uint8_t *data)
{
	for( int i=0 ; i<3 ; i++ )	HID_PS3_Data.button[i] = data[i+0x2];
	for( int i=0 ; i<4 ; i++ )	HID_PS3_Data.analog_stick[i] = data[i+0x6];
	for( int i=0 ; i<12 ; i++ )	HID_PS3_Data.button_analog[i] = data[i+0x0e];

	USR_PS3_ProcessData(&HID_PS3_Data);

}
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */


/**
 * @}
 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
