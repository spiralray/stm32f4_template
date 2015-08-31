/**
  ******************************************************************************
  * @file    usbh_hid_mouse.h
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   This file contains all the prototypes for the usbh_hid_mouse.c
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


/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_HID_PS3_H
#define __USBH_HID_PS3_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_conf.h"

#include "usb_bsp.h"
#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_hid_core.h"

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
  * @brief This file is the Header file for USBH_HID_PS3.c
  * @{
  */


/** @defgroup USBH_HID_PS3_Exported_Types
  * @{
  */
typedef struct _HID_PS3_Data
{
  uint8_t   button[3];
  uint8_t	analog_stick[4];
  uint8_t	button_analog[12];
}
HID_PS3_Data_TypeDef;

/**
  * @}
  */

/** @defgroup USBH_HID_PS3_Exported_Defines
  * @{
  */
/**
  * @}
  */

/* PS3 data taken from descriptors */
#define EP_MAXPKTSIZE       64 // max size for data via USB

/** Size of the output report buffer for the Dualshock and Navigation controllers */
#define PS3_REPORT_BUFFER_SIZE  48

/** Size of the output report buffer for the Move Controller */
#define MOVE_REPORT_BUFFER_SIZE 7

#define PS3_VID				0x054C
#define PS3_DUALSHOCK3_PID	0x0268


/** @defgroup USBH_HID_PS3_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_HID_PS3_Exported_Variables
  * @{
  */

extern HID_cb_TypeDef HID_PS3_cb;
extern HID_PS3_Data_TypeDef	 HID_PS3_Data;
/**
  * @}
  */

/** @defgroup USBH_HID_PS3_Exported_FunctionsPrototype
  * @{
  */
void  USR_PS3_Init (void);
void  USR_PS3_ProcessData (HID_PS3_Data_TypeDef *data);
/**
  * @}
  */

#endif /* __USBH_HID_PS3_H */

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
