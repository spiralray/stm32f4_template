/********************************************************************************/
/*!
	@file			uart_support.c
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
    @version        4.00
    @date           2014.05.01
	@brief          Based on ST Microelectronics Sample Thanks!

    @section HISTORY
		2011.06.12	V1.00	Start Here.
		2011.09.14	V2.00	Expand fifo buffer uint8_t to uint16_t
		2012.01.31	V3.00	UART IRQ Routine moved from stm32f4xx_it.c.
		2014.05.01	V4.00	Fixed Suitable Interrupt-Preemption level.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "utils/uart_support.h"
/* check header file version for fool proof */
#if __UART_SUPPORT_H!= 0x0400
#error "header file version is not correspond!"
#endif

/* Defines -------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
static USART_TypeDef* UART;
USART_Buffer_t* pUSART_Buf;

#if (UART_DEFAULT_NUM == 1)
USART_Buffer_t USART1_Buf;
#elif (UART_DEFAULT_NUM == 2)
USART_Buffer_t USART2_Buf;
#elif (UART_DEFAULT_NUM == 6)
USART_Buffer_t USART6_Buf;
#endif

/* Constants -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/**************************************************************************/
/*!
    Initialize UART.
*/
/**************************************************************************/
/* Initialize serial console */
void conio_init(uint32_t port, uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Turn on USART*/
	switch (port)
	{
#if (UART_DEFAULT_NUM == 1)
		case 1 :	/* UART1 Settings */
			UART = (USART_TypeDef *) USART1_BASE;

			/* Enable GPIO clock */
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			/* Enable UART clock */
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

			/* Connect PXx to USARTx_Tx*/
			GPIO_PinAFConfig(GPIOA,  GPIO_PinSource9, GPIO_AF_USART1);
			/* Connect PXx to USARTx_Rx*/
			GPIO_PinAFConfig(GPIOA,  GPIO_PinSource10, GPIO_AF_USART1);

			/* Configure USART Tx as alternate function  */
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			/* Configure USART Rx as alternate function  */
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

#if (UART_HANDLING == UART_INTERRUPT_MODE)
			/*
			 *NVIC_PriorityGroup_0: 0 Pre-emption priorities, 16 Sub-priorities
			 *NVIC_PriorityGroup_1: 2 Pre-emption priorities, 8 Sub-priorities
			 *NVIC_PriorityGroup_2: 4 Pre-emption priorities, 4 Sub-priorities
			 *NVIC_PriorityGroup_3: 8 Pre-emption priorities, 2 Sub-priorities
			 *NVIC_PriorityGroup_4: 16 Pre-emption priorities, 0 Sub-priorities
			 */
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

			/* Enable the USART Interrupt */
			NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			/* Init Ring Buffer */
			pUSART_Buf = &USART1_Buf;
			USART1_Buf.RX_Tail = 0;
			USART1_Buf.RX_Head = 0;
			USART1_Buf.TX_Tail = 0;
			USART1_Buf.TX_Head = 0;

			/* Enable USART1 Receive interrupts */
			USART_ITConfig(UART, USART_IT_RXNE, ENABLE);
#endif
			USART_StructInit(&USART_InitStructure);
			USART_InitStructure.USART_BaudRate = baudrate;
			USART_Init(UART, &USART_InitStructure);

			USART_Cmd(UART, ENABLE);
		break;
#elif (UART_DEFAULT_NUM == 2)
		case 2 :	/* UART2 Settings */
 			UART = (USART_TypeDef *) USART2_BASE;

			/* Enable GPIO clock */
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			/* Enable UART clock */
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

			/* Connect PXx to USARTx_Tx*/
			GPIO_PinAFConfig(GPIOA,  GPIO_PinSource2, GPIO_AF_USART2);
			/* Connect PXx to USARTx_Rx*/
			GPIO_PinAFConfig(GPIOA,  GPIO_PinSource3, GPIO_AF_USART2);

			/* Configure USART Tx as alternate function  */
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			/* Configure USART Rx as alternate function  */
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

#if (UART_HANDLING == UART_INTERRUPT_MODE)
			/*
			 *NVIC_PriorityGroup_0: 0 Pre-emption priorities, 16 Sub-priorities
			 *NVIC_PriorityGroup_1: 2 Pre-emption priorities, 8 Sub-priorities
			 *NVIC_PriorityGroup_2: 4 Pre-emption priorities, 4 Sub-priorities
			 *NVIC_PriorityGroup_3: 8 Pre-emption priorities, 2 Sub-priorities
			 *NVIC_PriorityGroup_4: 16 Pre-emption priorities, 0 Sub-priorities
			 */
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

			/* Enable the USART Interrupt */
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			/* Init Ring Buffer */
			pUSART_Buf = &USART2_Buf;
			USART2_Buf.RX_Tail = 0;
			USART2_Buf.RX_Head = 0;
			USART2_Buf.TX_Tail = 0;
			USART2_Buf.TX_Head = 0;

			/* Enable UART Receive interrupts */
			USART_ITConfig(UART, USART_IT_RXNE, ENABLE);
#endif
			USART_StructInit(&USART_InitStructure);
			USART_InitStructure.USART_BaudRate = baudrate;
			USART_Init(UART, &USART_InitStructure);

			USART_Cmd(UART, ENABLE);
		break;
#endif
#if (UART_DEFAULT_NUM == 6)
		case 6 :	/* Currentry not Supported! */
		  UART = (USART_TypeDef *) USART6_BASE;

		  /* Enable GPIO clock */
		  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		  /* Enable UART clock */
		  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

		  /* Connect PXx to USARTx_Tx*/
		  GPIO_PinAFConfig(GPIOC,  GPIO_PinSource6, GPIO_AF_USART6);
		  /* Connect PXx to USARTx_Rx*/
		  GPIO_PinAFConfig(GPIOC,  GPIO_PinSource7, GPIO_AF_USART6);

		  /* Configure USART Tx as alternate function  */
		  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

		  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		  GPIO_Init(GPIOC, &GPIO_InitStructure);

		  /* Configure USART Rx as alternate function  */
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
		  GPIO_Init(GPIOC, &GPIO_InitStructure);

#if (UART_HANDLING == UART_INTERRUPT_MODE)
		  /*
		   *NVIC_PriorityGroup_0: 0 Pre-emption priorities, 16 Sub-priorities
		   *NVIC_PriorityGroup_1: 2 Pre-emption priorities, 8 Sub-priorities
		   *NVIC_PriorityGroup_2: 4 Pre-emption priorities, 4 Sub-priorities
		   *NVIC_PriorityGroup_3: 8 Pre-emption priorities, 2 Sub-priorities
		   *NVIC_PriorityGroup_4: 16 Pre-emption priorities, 0 Sub-priorities
		   */
		  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

		  /* Enable the USART Interrupt */
		  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
		  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		  NVIC_Init(&NVIC_InitStructure);

		  /* Init Ring Buffer */
		  pUSART_Buf = &USART6_Buf;
		  USART6_Buf.RX_Tail = 0;
		  USART6_Buf.RX_Head = 0;
		  USART6_Buf.TX_Tail = 0;
		  USART6_Buf.TX_Head = 0;

		  /* Enable USART1 Receive interrupts */
		  USART_ITConfig(UART, USART_IT_RXNE, ENABLE);
#endif
		  USART_StructInit(&USART_InitStructure);
		  USART_InitStructure.USART_BaudRate = 115200;
		  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		  USART_InitStructure.USART_StopBits = USART_StopBits_1;
		  USART_InitStructure.USART_Parity = USART_Parity_No ;
		  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		  USART_Init(UART, &USART_InitStructure);

		  USART_Cmd(UART, ENABLE);
		break;
#endif
	}

}

/**************************************************************************/
/*!
    Check UART TX Buffer Empty.
*/
/**************************************************************************/
bool USART_TXBuffer_FreeSpace(USART_Buffer_t* USART_buf)
{
	/* Make copies to make sure that volatile access is specified. */
	uint16_t tempHead = (USART_buf->TX_Head + 1) & (UART_BUFSIZE-1);
	uint16_t tempTail = USART_buf->TX_Tail;

	/* There are data left in the buffer unless Head and Tail are equal. */
	return (tempHead != tempTail);
}

/**************************************************************************/
/*!
    Put Bytedata with Buffering.
*/
/**************************************************************************/
bool USART_TXBuffer_PutByte(USART_Buffer_t* USART_buf, uint8_t data)
{

	uint16_t tempTX_Head;
	bool TXBuffer_FreeSpace;

	TXBuffer_FreeSpace = USART_TXBuffer_FreeSpace(USART_buf);


	if(TXBuffer_FreeSpace)
	{
	  	tempTX_Head = USART_buf->TX_Head;

		__disable_irq();
	  	USART_buf->TX[tempTX_Head]= data;
		/* Advance buffer head. */
		USART_buf->TX_Head = (tempTX_Head + 1) & (UART_BUFSIZE-1);
		__enable_irq();

		/* Enable TXE interrupt. */
		USART_ITConfig(UART, USART_IT_TXE, ENABLE);
	}
	return TXBuffer_FreeSpace;
}

/**************************************************************************/
/*!
    Check UART RX Buffer Empty.
*/
/**************************************************************************/
bool USART_RXBufferData_Available(USART_Buffer_t* USART_buf)
{
	/* Make copies to make sure that volatile access is specified. */
	uint16_t tempHead = USART_buf->RX_Head;
	uint16_t tempTail = USART_buf->RX_Tail;

	/* There are data left in the buffer unless Head and Tail are equal. */
	return (tempHead != tempTail);
}

/**************************************************************************/
/*!
    Get Bytedata with Buffering.
*/
/**************************************************************************/
uint8_t USART_RXBuffer_GetByte(USART_Buffer_t* USART_buf)
{
	uint8_t ans;

	__disable_irq();
	ans = (USART_buf->RX[USART_buf->RX_Tail]);

	/* Advance buffer tail. */
	USART_buf->RX_Tail = (USART_buf->RX_Tail + 1) & (UART_BUFSIZE-1);

	__enable_irq();

	return ans;
}

/**************************************************************************/
/*!
    High Level function.
*/
/**************************************************************************/
/* Send 1 character */
inline void putch(uint8_t data)
{
#if (UART_HANDLING == UART_INTERRUPT_MODE)
	/* Interrupt Version */
	while(!USART_TXBuffer_FreeSpace(pUSART_Buf));
	USART_TXBuffer_PutByte(pUSART_Buf,data);
#else
	/* Polling version */
	while (!(UART->SR & USART_FLAG_TXE));
	UART->DR = data;
#endif
}

/**************************************************************************/
/*!
    High Level function.
*/
/**************************************************************************/
/* Receive 1 character */
uint8_t getch(void)
{
#if (UART_HANDLING == UART_INTERRUPT_MODE)
	if (USART_RXBufferData_Available(pUSART_Buf))   return USART_RXBuffer_GetByte(pUSART_Buf);
	else						return false;
#else
	/* Polling version */
	while (!(UART->SR & USART_FLAG_RXNE));
	return (uint8_t)(USART->DR);
#endif
}

/**************************************************************************/
/*!
    High Level function.
*/
/**************************************************************************/
/* Return 1 if key pressed */
uint8_t keypressed(void)
{
#if (UART_HANDLING == UART_INTERRUPT_MODE)
  return (USART_RXBufferData_Available(pUSART_Buf));
#else
  return (UART->SR & USART_FLAG_RXNE);
#endif
}

/**************************************************************************/
/*!
    High Level function.
*/
/**************************************************************************/
/* Send a string */
void cputs(char *s)
{
  while (*s)
    putch(*s++);
}

/**************************************************************************/
/*!
    High Level function.
*/
/**************************************************************************/
/* Receive a string, with rudimentary line editing */
void cgets(char *s, int bufsize)
{
  char *p;
  int c;

  memset(s, 0, bufsize);

  p = s;

  for (p = s; p < s + bufsize-1;)
  {
    /* 20090521Nemui */
	do{
		c = getch();
	}while(c == false);
	/* 20090521Nemui */
    switch (c)
    {
      case '\r' :
      case '\n' :
        putch('\r');
        putch('\n');
        *p = '\n';
        return;

      case '\b' :
        if (p > s)
        {
          *p-- = 0;
          putch('\b');
          putch(' ');
          putch('\b');
        }
        break;

      default :
        putch(c);
        *p++ = c;
        break;
    }
  }

  return;
}

#if (UART_DEFAULT_NUM == 1)
/**************************************************************************/
/*!
    @brief	Handles USART1 global interrupt request.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
void USART1_IRQHandler(void)
{

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		/* Advance buffer head. */
		uint16_t tempRX_Head = ((&USART1_Buf)->RX_Head + 1) & (UART_BUFSIZE-1);

		/* Check for overflow. */
		uint16_t tempRX_Tail = (&USART1_Buf)->RX_Tail;
		uint8_t data =  USART_ReceiveData(USART1);

		if (tempRX_Head == tempRX_Tail) {
			/* Disable the USART1 Receive interrupt */
			USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
		}else{
			(&USART1_Buf)->RX[(&USART1_Buf)->RX_Head] = data;
			(&USART1_Buf)->RX_Head = tempRX_Head;
		}

	}

	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{
		/* Check if all data is transmitted. */
		uint16_t tempTX_Tail = (&USART1_Buf)->TX_Tail;
		if ((&USART1_Buf)->TX_Head == tempTX_Tail){
			/* Overflow MAX size Situation */
			/* Disable the USART1 Transmit interrupt */
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		}else{
			/* Start transmitting. */
			uint8_t data = (&USART1_Buf)->TX[(&USART1_Buf)->TX_Tail];
			USART1->DR = data;

			/* Advance buffer tail. */
			(&USART1_Buf)->TX_Tail = ((&USART1_Buf)->TX_Tail + 1) & (UART_BUFSIZE-1);
		}

	}
}

#elif (UART_DEFAULT_NUM == 2)
/**************************************************************************/
/*!
    @brief	Handles USART2 global interrupt request.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
void USART2_IRQHandler(void)
{

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		/* Advance buffer head. */
		uint16_t tempRX_Head = ((&USART2_Buf)->RX_Head + 1) & (UART_BUFSIZE-1);

		/* Check for overflow. */
		uint16_t tempRX_Tail = (&USART2_Buf)->RX_Tail;
		uint8_t data =  USART_ReceiveData(USART2);

		if (tempRX_Head == tempRX_Tail) {
			/* Disable the USART2 Receive interrupt */
			USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
		}else{
			(&USART2_Buf)->RX[(&USART2_Buf)->RX_Head] = data;
			(&USART2_Buf)->RX_Head = tempRX_Head;
		}

	}

	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
	{
		/* Check if all data is transmitted. */
		uint16_t tempTX_Tail = (&USART2_Buf)->TX_Tail;
		if ((&USART2_Buf)->TX_Head == tempTX_Tail){
			/* Overflow MAX size Situation */
			/* Disable the USART2 Transmit interrupt */
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		}else{
			/* Start transmitting. */
			uint8_t data = (&USART2_Buf)->TX[(&USART2_Buf)->TX_Tail];
			USART2->DR = data;

			/* Advance buffer tail. */
			(&USART2_Buf)->TX_Tail = ((&USART2_Buf)->TX_Tail + 1) & (UART_BUFSIZE-1);
		}

	}
}

#elif (UART_DEFAULT_NUM == 6)
/**************************************************************************/
/*!
    @brief      Handles USART6 global interrupt request.
        @param  None.
    @retval     None.
*/
/**************************************************************************/
void USART6_IRQHandler(void)
{

        if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
        {
                /* Advance buffer head. */
                uint16_t tempRX_Head = ((&USART6_Buf)->RX_Head + 1) & (UART_BUFSIZE-1);

                /* Check for overflow. */
                uint16_t tempRX_Tail = (&USART6_Buf)->RX_Tail;
                uint8_t data =  USART_ReceiveData(USART6);

                if (tempRX_Head == tempRX_Tail) {
                        /* Disable the USART6 Receive interrupt */
                        USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);
                }else{
                        (&USART6_Buf)->RX[(&USART6_Buf)->RX_Head] = data;
                        (&USART6_Buf)->RX_Head = tempRX_Head;
                }

        }

        if(USART_GetITStatus(USART6, USART_IT_TXE) != RESET)
        {
                /* Check if all data is transmitted. */
                uint16_t tempTX_Tail = (&USART6_Buf)->TX_Tail;
                if ((&USART6_Buf)->TX_Head == tempTX_Tail){
                        /* Overflow MAX size Situation */
                        /* Disable the USART6 Transmit interrupt */
                        USART_ITConfig(USART6, USART_IT_TXE, DISABLE);
                }else{
                        /* Start transmitting. */
                        uint8_t data = (&USART6_Buf)->TX[(&USART6_Buf)->TX_Tail];
                        USART6->DR = data;

                        /* Advance buffer tail. */
                        (&USART6_Buf)->TX_Tail = ((&USART6_Buf)->TX_Tail + 1) & (UART_BUFSIZE-1);
                }

        }
}
#endif

/* End Of File ---------------------------------------------------------------*/
