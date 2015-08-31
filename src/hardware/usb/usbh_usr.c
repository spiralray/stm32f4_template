/**
 ******************************************************************************
 * @file    usbh_usr.c
 * @author  MCD Application Team and modified by Yuuichi Akagawa
 * @version V2.0.0
 * @date    22-July-2011, 2012/03/05
 * @brief   This file includes the user application layer
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usbh_usr.h"
#include "usbh_hid_ps3.h"
#include "stm32f4xx.h"


/** @addtogroup USBH_USER
 * @{
 */

/** @addtogroup USBH_HID_DEMO_USER_CALLBACKS
 * @{
 */

/** @defgroup USBH_USR
 * @brief This file is the Header file for usbh_usr.c
 * @{
 */


/** @defgroup USBH_CORE_Exported_TypesDefinitions
 * @{
 */


/**
 * @}
 */

/** @addtogroup USER
 * @{
 */

/** @defgroup USBH_USR
 * @brief    This file includes the user application layer
 * @{
 */

/** @defgroup USBH_CORE_Exported_Types
 * @{
 */



/** @defgroup USBH_USR_Private_Defines
 * @{
 */
/**
 * @}
 */


/** @defgroup USBH_USR_Private_Macros
 * @{
 */
/**
 * @}
 */

/** @defgroup USBH_USR_Private_Variables
 * @{
 */
extern USB_OTG_CORE_HANDLE           USB_OTG_Core_dev;
/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USR_Callbacks =
{
		USBH_USR_Init,
		USBH_USR_DeInit,
		USBH_USR_DeviceAttached,
		USBH_USR_ResetDevice,
		USBH_USR_DeviceDisconnected,
		USBH_USR_OverCurrentDetected,
		USBH_USR_DeviceSpeedDetected,
		USBH_USR_Device_DescAvailable,
		USBH_USR_DeviceAddressAssigned,
		USBH_USR_Configuration_DescAvailable,
		USBH_USR_Manufacturer_String,
		USBH_USR_Product_String,
		USBH_USR_SerialNum_String,
		USBH_USR_EnumerationDone,
		USBH_USR_UserInput,
		NULL,
		USBH_USR_DeviceNotSupported,
		USBH_USR_UnrecoveredError
};

volatile uint8_t ps3_connected = 0;

/**
 * @}
 */

/** @defgroup USBH_USR_Private_Constants
 * @{
 */
/*--------------- LCD Messages ---------------*/
const uint8_t MSG_HOST_INIT[]          = "> Host Library Initialized\n";
const uint8_t MSG_DEV_ATTACHED[]       = "> Device Attached\n";
const uint8_t MSG_DEV_DISCONNECTED[]   = "> Device Disconnected\n";
const uint8_t MSG_DEV_ENUMERATED[]     = "> Enumeration completed\n";
const uint8_t MSG_DEV_HIGHSPEED[]      = "> High speed device detected\n";
const uint8_t MSG_DEV_FULLSPEED[]      = "> Full speed device detected\n";
const uint8_t MSG_DEV_LOWSPEED[]       = "> Low speed device detected\n";
const uint8_t MSG_DEV_ERROR[]          = "> Device fault \n";

const uint8_t MSG_MSC_CLASS[]          = "> Mass storage device connected\n";
const uint8_t MSG_HID_CLASS[]          = "> HID device connected\n";

const uint8_t USB_HID_MouseStatus[]    = "> HID Demo Device : Mouse\n";
const uint8_t USB_HID_KeybrdStatus[]   = "> HID Demo Device : Keyboard\n";
const uint8_t MSG_UNREC_ERROR[]        = "> UNRECOVERED ERROR STATE\n";
/**
 * @}
 */



/** @defgroup USBH_USR_Private_FunctionPrototypes
 * @{
 */
/**
 * @}
 */


/** @defgroup USBH_USR_Private_Functions
 * @{
 */





/**
 * @brief  USBH_USR_Init
 *         Displays the message on LCD for host lib initialization
 * @param  None
 * @retval None
 */
void USBH_USR_Init(void)
{

}

/**
 * @brief  USBH_USR_DeviceAttached
 *         Displays the message on LCD on device attached
 * @param  None
 * @retval None
 */
void USBH_USR_DeviceAttached(void)
{
	//printf("Device Attached \n");
}

/**
 * @brief  USBH_USR_UnrecoveredError
 * @param  None
 * @retval None
 */
void USBH_USR_UnrecoveredError (void)
{

}

/**
 * @brief  USBH_DisconnectEvent
 *         Device disconnect event
 * @param  None
 * @retval None
 */
void USBH_USR_DeviceDisconnected (void)
{
	//printf("Device Disconnected \n");
	ps3_connected = 0;
}

/**
 * @brief  USBH_USR_ResetUSBDevice
 *         Reset USB Device
 * @param  None
 * @retval None
 */
void USBH_USR_ResetDevice(void)
{
	/* Users can do their application actions here for the USB-Reset */
}


/**
 * @brief  USBH_USR_DeviceSpeedDetected
 *         Displays the message on LCD for device speed
 * @param  Devicespeed : Device Speed
 * @retval None
 */
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
	if(DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED)
	{
		//printf("%s\n", MSG_DEV_HIGHSPEED);
	}
	else if(DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
	{
		//printf("%s\n", MSG_DEV_FULLSPEED);
	}
	else if(DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
	{
		//printf("%s\n", MSG_DEV_LOWSPEED);
	}
	else
	{
		//printf("%s\n", MSG_DEV_ERROR);
	}
}

/**
 * @brief  USBH_USR_Device_DescAvailable
 *         Displays the message on LCD for device descriptor
 * @param  DeviceDesc : device descriptor
 * @retval None
 */
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
	USBH_DevDesc_TypeDef *hs;
	hs = DeviceDesc;

	//printf("VID : %04Xh\n" , (uint32_t)(*hs).idVendor);
	//printf("PID : %04Xh\n" , (uint32_t)(*hs).idProduct);

}

/**
 * @brief  USBH_USR_DeviceAddressAssigned
 *         USB device is successfully assigned the Address
 * @param  None
 * @retval None
 */
void USBH_USR_DeviceAddressAssigned(void)
{

}


/**
 * @brief  USBH_USR_Conf_Desc
 *         Displays the message on LCD for configuration descriptor
 * @param  ConfDesc : Configuration descriptor
 * @retval None
 */
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
		USBH_InterfaceDesc_TypeDef *itfDesc,
		USBH_EpDesc_TypeDef *epDesc)
{
	USBH_InterfaceDesc_TypeDef *id;

	id = itfDesc;

	if((*id).bInterfaceClass  == 0x08)
	{
		//printf("%s\n", MSG_MSC_CLASS);
	}
	else if((*id).bInterfaceClass  == 0x03)
	{

		//printf("%s\n", MSG_HID_CLASS);
	}
}

/**
 * @brief  USBH_USR_Manufacturer_String
 *         Displays the message on LCD for Manufacturer String
 * @param  ManufacturerString : Manufacturer String of Device
 * @retval None
 */
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
	//printf("Manufacturer : %s\n", (char *)ManufacturerString);
}

/**
 * @brief  USBH_USR_Product_String
 *         Displays the message on LCD for Product String
 * @param  ProductString : Product String of Device
 * @retval None
 */
void USBH_USR_Product_String(void *ProductString)
{
	//printf("Product : %s\n", (char *)ProductString);
}

/**
 * @brief  USBH_USR_SerialNum_String
 *         Displays the message on LCD for SerialNum_String
 * @param  SerialNumString : SerialNum_String of device
 * @retval None
 */
void USBH_USR_SerialNum_String(void *SerialNumString)
{
	//printf("Serial Number : %s\n", (char *)SerialNumString);
}

/**
 * @brief  EnumerationDone
 *         User response request is displayed to ask for
 *         application jump to class
 * @param  None
 * @retval None
 */
void USBH_USR_EnumerationDone(void)
{
	//STM_EVAL_LEDOn(LED3);
	//printf("Enumeration done! \n");
}

/**
 * @brief  USBH_USR_DeviceNotSupported
 *         Device is not supported
 * @param  None
 * @retval None
 */
void USBH_USR_DeviceNotSupported(void)
{
	//printf("Device is not supported! \n");
}


/**
 * @brief  USBH_USR_UserInput
 *         User Action for application state entry
 * @param  None
 * @retval USBH_USR_Status : User response for key button
 */
USBH_USR_Status USBH_USR_UserInput(void)
{

	return USBH_USR_RESP_OK;

}

/**
 * @brief  USBH_USR_OverCurrentDetected
 *         Device Overcurrent detection event
 * @param  None
 * @retval None
 */
void USBH_USR_OverCurrentDetected (void)
{

}

/**
 * @brief  USR_PS3_Init
 *         Init Keyboard window
 * @param  None
 * @retval None
 */
void  USR_PS3_Init (void)
{
	//printf("PS3 init\n");
	ps3_connected = 1;
}


/**
 * @brief  USR_PS3_ProcessData
 *         Process Keyboard data
 * @param  data : Keyboard data to be displayed
 * @retval None
 */
void  USR_PS3_ProcessData (HID_PS3_Data_TypeDef *data)
{
#if 0
	printf("Button state 0x%02x 0x%02x 0x%02x \n", data->button[0], data->button[1], data->button[2] );
	printf("Analog state 0x%02x 0x%02x 0x%02x 0x%02x \n", data->analog_stick[0], data->analog_stick[1], data->analog_stick[2], data->analog_stick[3] );

	printf("Analog state ");
	for( int i=0 ; i<12 ; i++ ) printf("%02x ", data->button_analog[i]);
	printf("\n");

	printf("\n");
#endif
}

/**
 * @brief  USBH_USR_DeInit
 *         Deint User state and associated variables
 * @param  None
 * @retval None
 */
void USBH_USR_DeInit(void)
{
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
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
