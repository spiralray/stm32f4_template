/*
 * can.cpp
 *
 *  Created on: 2014/11/01
 *      Author: Kentaro Tanaka
 */

/* Includes ------------------------------------------------------------------*/
#include "hardware/can.h"

CAN_Buffer_t CAN1_Buf;

void CAN1_Configuration(){
  GPIO_InitTypeDef  GPIO_InitStructure;

  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;

  /* CAN GPIOs configuration **************************************************/

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD , ENABLE);

  /* Connect CAN pins to AF9 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1 , GPIO_AF_CAN1);

  /* Configure CAN RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* CAN configuration ********************************************************/
  /* Enable CAN clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

  /* CAN register init */
  CAN_DeInit(CAN1);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;

  /* CAN Baudrate = 1 MBps (CAN clocked at 42 MHz APB1 Clock) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;	//6
  CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;	//7
  CAN_InitStructure.CAN_Prescaler = 7-1;		// 42/3=14	14/(6+7+1)=1MHz
  CAN_Init(CAN1, &CAN_InitStructure);

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  NVIC_InitTypeDef  NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  CAN1_Buf.CAN = CAN1;
  CAN1_Buf.RX_Tail = 0;
  CAN1_Buf.RX_Head = 0;

  /* Enable FIFO 0 message pending Interrupt */
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

/**************************************************************************/
/*!
    Check CAN RX Buffer Empty.
 */
/**************************************************************************/
uint8_t CAN_RXBufferData_Available(CAN_Buffer_t* CAN_buf)
{
  /* Make copies to make sure that volatile access is specified. */
  uint16_t tempHead = CAN_buf->RX_Head;
  uint16_t tempTail = CAN_buf->RX_Tail;

  /* There are data left in the buffer unless Head and Tail are equal. */
  return (tempHead != tempTail);
}

/**************************************************************************/
/*!
    Get CanRxMsg with Buffering.
 */
/**************************************************************************/
CanRxMsg CAN_RXBuffer_GetData(CAN_Buffer_t* CAN_buf)
{
  CanRxMsg ans;

  __disable_irq();
  ans = (CAN_buf->RX[CAN_buf->RX_Tail]);

  /* Advance buffer tail. */
  CAN_buf->RX_Tail = (CAN_buf->RX_Tail + 1) & (CAN_BUFSIZE-1);

  __enable_irq();

  return ans;
}

void CAN1_RX0_IRQHandler() {

  /* Advance buffer head. */
  uint16_t tempRX_Head = ((&CAN1_Buf)->RX_Head + 1) & (CAN_BUFSIZE-1);

  /* Check for overflow. */
  uint16_t tempRX_Tail = (&CAN1_Buf)->RX_Tail;
  CanRxMsg data;
  CAN_Receive(CAN1, CAN_FIFO0, &data);

  if (tempRX_Head == tempRX_Tail) {
      /* Disable the CAN1 Receive interrupt */
      CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);
  }else{
      (&CAN1_Buf)->RX[(&CAN1_Buf)->RX_Head] = data;
      (&CAN1_Buf)->RX_Head = tempRX_Head;
  }

  __DSB();      // prevent erroneous recall of this handler due to delayed memory write
}
