/********************************************************************************/
/*!
	@file			stm32f4xx_it.c
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
    @version        1.00
    @date           2011.10.16
	@brief          Cortex-M4 Processor Exceptions Handlers.				@n
					And STM32F4xx Peripherals Interrupt Handlers.			@n
					Device Dependent Section.

    @section HISTORY
		2011.10.16	V1.00	Start Here.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "hw_config.h"

#ifdef USE_USB
#include "usb_bsp.h"
#endif

/* Defines -------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/
#ifdef USE_USB
extern USB_OTG_CORE_HANDLE          USB_OTG_Core_dev;
#endif

/* Constants -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/
#ifdef USE_ACCURATE_TIME
extern void USB_OTG_BSP_TimerIRQ (void);
#endif

/* Functions -----------------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**************************************************************************/
/*!
    @brief	Handles NMI exception.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
void NMI_Handler(void)
{
}


/**************************************************************************/
/*!
    @brief	Handles Hard Fault exception.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}


/**************************************************************************/
/*!
    @brief	Handles Memory Manage exception.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}


/**************************************************************************/
/*!
    @brief	Handles Bus Fault exception.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}


/**************************************************************************/
/*!
    @brief	Handles Usage Fault exception.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}


/**************************************************************************/
/*!
    @brief	Handles SVCall exception.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
#ifndef USE_FREERTOS
void SVC_Handler(void)
{
}
#endif

/**************************************************************************/
/*!
    @brief	Handles Debug Monitor exception.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
void DebugMon_Handler(void)
{
}


/**************************************************************************/
/*!
    @brief	Handles PendSVC exception.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
#ifndef USE_FREERTOS
void PendSV_Handler(void)
{
}
#endif

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32F4xx_xx.s).                                            */
/******************************************************************************/

/**************************************************************************/
/*!
    @brief	Handles RealTimeClock interrupts requests.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
void RTC_IRQHandler(void)
{
	/* Put Your Code Here. */
}

#ifdef USE_USB
#ifdef USE_ACCURATE_TIME
/**
  * @brief  TIM2_IRQHandler
  *         This function handles Timer2 Handler.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
  USB_OTG_BSP_TimerIRQ();
}
#endif

/**
  * @brief  OTG_FS_IRQHandler
  *          This function handles USB-On-The-Go FS global interrupt request.
  *          requests.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_OTG_FS
void OTG_FS_IRQHandler(void) {
#else
void OTG_HS_IRQHandler(void) {
#endif

  USBH_OTG_ISR_Handler(&USB_OTG_Core_dev);
}
#endif

/* End Of File ---------------------------------------------------------------*/
