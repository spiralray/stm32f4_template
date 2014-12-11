#pragma once

#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif

#include "config/stdperiph.h"

//------------------------------------------------------------------------------
#define CAN_BUFSIZE 64        //must be a number power of 2

typedef struct CAN_Buffer
{
  CAN_TypeDef * CAN;
  /* @brief Receive buffer. */
  volatile CanRxMsg RX[CAN_BUFSIZE];
  /* @brief Receive buffer head. */
  volatile uint16_t RX_Head;
  /* @brief Receive buffer tail. */
  volatile uint16_t RX_Tail;
} CAN_Buffer_t;


extern CAN_Buffer_t CAN1_Buf;
//------------------------------------------------------------------------------

void CAN1_Configuration();

uint8_t CAN_RXBufferData_Available(CAN_Buffer_t* CAN_buf);
CanRxMsg CAN_RXBuffer_GetData(CAN_Buffer_t* CAN_buf);

#ifdef __cplusplus    /* Insert start of extern C construct */
}
#endif
