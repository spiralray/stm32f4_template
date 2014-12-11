/**************************************************************************/
/*!
	@file			stm32f4xx_io_view.c
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
    @version        3.00
    @date           2014.05.08
	@brief          Debugging I/O Definitions For STM32F4xxx Devices.

    @section HISTORY
		2011.10.19	V1.00	Start Here.
		2013.11.30	V2.00	Added STM32F42xxxx and STM32F43xxxx Devices.
		2014.05.08	V3.00	Fixed for STM32F401xx Devices.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Defines -------------------------------------------------------------------*/
typedef union GPIODATA_t {

	__IO uint16_t DUMMY;
	struct {
		__IO	uint16_t DR_00:	1;
		__IO	uint16_t DR_01:	1;
		__IO	uint16_t DR_02:	1;
		__IO	uint16_t DR_03:	1;
		__IO	uint16_t DR_04:	1;
		__IO	uint16_t DR_05:	1;
		__IO	uint16_t DR_06:	1;
		__IO	uint16_t DR_07:	1;
		__IO	uint16_t DR_08:	1;
		__IO	uint16_t DR_09:	1;
		__IO	uint16_t DR_10:	1;
		__IO	uint16_t DR_11:	1;
		__IO	uint16_t DR_12:	1;
		__IO	uint16_t DR_13:	1;
		__IO	uint16_t DR_14:	1;
		__IO	uint16_t DR_15:	1;
	} b_DR;
} GPIODATA_t;


/* Constants -----------------------------------------------------------------*/
/* SANKOU SHIRYOU */
//typedef union
//{
//  struct
//  {
//    uint32_t ISR:9;                      /*!< bit:  0.. 8  Exception number                   */
//#if (__CORTEX_M != 0x04)
//    uint32_t _reserved0:15;              /*!< bit:  9..23  Reserved                           */
//#else
//    uint32_t _reserved0:7;               /*!< bit:  9..15  Reserved                           */
//    uint32_t GE:4;                       /*!< bit: 16..19  Greater than or Equal flags        */
//    uint32_t _reserved1:4;               /*!< bit: 20..23  Reserved                           */
//#endif
//    uint32_t T:1;                        /*!< bit:     24  Thumb bit        (read 0)          */
//    uint32_t IT:2;                       /*!< bit: 25..26  saved IT state   (read 0)          */
//    uint32_t Q:1;                        /*!< bit:     27  Saturation condition flag          */
//    uint32_t V:1;                        /*!< bit:     28  Overflow condition code flag       */
//    uint32_t C:1;                        /*!< bit:     29  Carry condition code flag          */
//    uint32_t Z:1;                        /*!< bit:     30  Zero condition code flag           */
//    uint32_t N:1;                        /*!< bit:     31  Negative condition code flag       */
//  } b;                                   /*!< Structure used for bit  access                  */
//  uint32_t w;                            /*!< Type      used for word access                  */
//} xPSR_Type;
volatile SCnSCB_Type     * const IO_SCnSCB         __attribute__ ((section(".ioview"))) = SCnSCB;
volatile SCB_Type        * const IO_SCB            __attribute__ ((section(".ioview"))) = SCB;
volatile SysTick_Type    * const IO_SysTick        __attribute__ ((section(".ioview"))) = SysTick;
volatile NVIC_Type       * const IO_NVIC           __attribute__ ((section(".ioview"))) = NVIC;
volatile ITM_Type        * const IO_ITM            __attribute__ ((section(".ioview"))) = ITM;
volatile DWT_Type        * const IO_DWT            __attribute__ ((section(".ioview"))) = DWT;
volatile TPI_Type        * const IO_TPI            __attribute__ ((section(".ioview"))) = TPI;
volatile CoreDebug_Type  * const IO_CoreDebug      __attribute__ ((section(".ioview"))) = CoreDebug;

#if (__MPU_PRESENT == 1)
 volatile MPU_Type        * const IO_MPU            __attribute__ ((section(".ioview"))) = MPU;
#endif

#if (__FPU_PRESENT == 1)
 volatile FPU_Type        * const IO_FPU            __attribute__ ((section(".ioview"))) = FPU;
#endif


volatile ADC_Common_TypeDef   * const IO_ADC			__attribute__ ((section(".ioview"))) = ADC;
volatile ADC_TypeDef          * const IO_ADC1			__attribute__ ((section(".ioview"))) = ADC1;
volatile ADC_TypeDef          * const IO_ADC2			__attribute__ ((section(".ioview"))) = ADC2;
volatile ADC_TypeDef          * const IO_ADC3			__attribute__ ((section(".ioview"))) = ADC3;
volatile CAN_TypeDef          * const IO_CAN1			__attribute__ ((section(".ioview"))) = CAN1;
volatile CAN_TypeDef          * const IO_CAN2			__attribute__ ((section(".ioview"))) = CAN2;
volatile CRC_TypeDef          * const IO_CRC			__attribute__ ((section(".ioview"))) = CRC;
volatile CRYP_TypeDef         * const IO_CRYP			__attribute__ ((section(".ioview"))) = CRYP;
volatile DAC_TypeDef          * const IO_DAC			__attribute__ ((section(".ioview"))) = DAC;
volatile DBGMCU_TypeDef       * const IO_DBGMCU			__attribute__ ((section(".ioview"))) = DBGMCU;
volatile DCMI_TypeDef         * const IO_DCMI			__attribute__ ((section(".ioview"))) = DCMI;
volatile DMA_TypeDef          * const IO_DMA1			__attribute__ ((section(".ioview"))) = DMA1;
volatile DMA_Stream_TypeDef   * const IO_DMA1_Stream0	__attribute__ ((section(".ioview"))) = DMA1_Stream0;
volatile DMA_Stream_TypeDef   * const IO_DMA1_Stream1	__attribute__ ((section(".ioview"))) = DMA1_Stream1;
volatile DMA_Stream_TypeDef   * const IO_DMA1_Stream2	__attribute__ ((section(".ioview"))) = DMA1_Stream2;
volatile DMA_Stream_TypeDef   * const IO_DMA1_Stream3	__attribute__ ((section(".ioview"))) = DMA1_Stream3;
volatile DMA_Stream_TypeDef   * const IO_DMA1_Stream4	__attribute__ ((section(".ioview"))) = DMA1_Stream4;
volatile DMA_Stream_TypeDef   * const IO_DMA1_Stream5	__attribute__ ((section(".ioview"))) = DMA1_Stream5;
volatile DMA_Stream_TypeDef   * const IO_DMA1_Stream6	__attribute__ ((section(".ioview"))) = DMA1_Stream6;
volatile DMA_Stream_TypeDef   * const IO_DMA1_Stream7	__attribute__ ((section(".ioview"))) = DMA1_Stream7;
volatile DMA_TypeDef          * const IO_DMA2			__attribute__ ((section(".ioview"))) = DMA2;
volatile DMA_Stream_TypeDef   * const IO_DMA2_Stream0	__attribute__ ((section(".ioview"))) = DMA2_Stream0;
volatile DMA_Stream_TypeDef   * const IO_DMA2_Stream1	__attribute__ ((section(".ioview"))) = DMA2_Stream1;
volatile DMA_Stream_TypeDef   * const IO_DMA2_Stream2	__attribute__ ((section(".ioview"))) = DMA2_Stream2;
volatile DMA_Stream_TypeDef   * const IO_DMA2_Stream3	__attribute__ ((section(".ioview"))) = DMA2_Stream3;
volatile DMA_Stream_TypeDef   * const IO_DMA2_Stream4	__attribute__ ((section(".ioview"))) = DMA2_Stream4;
volatile DMA_Stream_TypeDef   * const IO_DMA2_Stream5	__attribute__ ((section(".ioview"))) = DMA2_Stream5;
volatile DMA_Stream_TypeDef   * const IO_DMA2_Stream6	__attribute__ ((section(".ioview"))) = DMA2_Stream6;
volatile DMA_Stream_TypeDef   * const IO_DMA2_Stream7	__attribute__ ((section(".ioview"))) = DMA2_Stream7;
volatile EXTI_TypeDef         * const IO_EXTI			__attribute__ ((section(".ioview"))) = EXTI;
volatile ETH_TypeDef          * const IO_ETH			__attribute__ ((section(".ioview"))) = ETH;
volatile FLASH_TypeDef        * const IO_FLASH			__attribute__ ((section(".ioview"))) = FLASH;
#if defined(STM32F40_41xxx)
volatile FSMC_Bank1_TypeDef   * const IO_FSMC_Bank1		__attribute__ ((section(".ioview"))) = FSMC_Bank1;
volatile FSMC_Bank1E_TypeDef  * const IO_FSMC_Bank1E	__attribute__ ((section(".ioview"))) = FSMC_Bank1E;
volatile FSMC_Bank2_TypeDef   * const IO_FSMC_Bank2		__attribute__ ((section(".ioview"))) = FSMC_Bank2;
volatile FSMC_Bank3_TypeDef   * const IO_FSMC_Bank3		__attribute__ ((section(".ioview"))) = FSMC_Bank3;
volatile FSMC_Bank4_TypeDef   * const IO_FSMC_Bank4		__attribute__ ((section(".ioview"))) = FSMC_Bank4;
#elif defined (STM32F427_437xx) || defined (STM32F429_439xx)
volatile FMC_Bank1_TypeDef   * const IO_FMC_Bank1		__attribute__ ((section(".ioview"))) = FMC_Bank1;
volatile FMC_Bank1E_TypeDef  * const IO_FMC_Bank1E		__attribute__ ((section(".ioview"))) = FMC_Bank1E;
volatile FMC_Bank2_TypeDef   * const IO_FMC_Bank2		__attribute__ ((section(".ioview"))) = FMC_Bank2;
volatile FMC_Bank3_TypeDef   * const IO_FMC_Bank3		__attribute__ ((section(".ioview"))) = FMC_Bank3;
volatile FMC_Bank4_TypeDef   * const IO_FMC_Bank4		__attribute__ ((section(".ioview"))) = FMC_Bank4;
volatile FMC_Bank5_6_TypeDef * const IO_FMC_Bank5_6		__attribute__ ((section(".ioview"))) = FMC_Bank5_6;
volatile SAI_TypeDef   		 * const IO_SAI1			__attribute__ ((section(".ioview"))) = SAI1;
volatile SAI_Block_TypeDef   * const IO_SAI1_Block_A	__attribute__ ((section(".ioview"))) = SAI1_Block_A;
volatile SAI_Block_TypeDef   * const IO_SAI1_Block_B	__attribute__ ((section(".ioview"))) = SAI1_Block_B;
volatile LTDC_TypeDef   	 * const IO_LTDC			__attribute__ ((section(".ioview"))) = LTDC;
volatile LTDC_Layer_TypeDef  * const IO_LTDC_Layer1		__attribute__ ((section(".ioview"))) = LTDC_Layer1;
volatile LTDC_Layer_TypeDef  * const IO_LTDC_Layer2		__attribute__ ((section(".ioview"))) = LTDC_Layer2;
volatile DMA2D_TypeDef   	 * const IO_DMA2D			__attribute__ ((section(".ioview"))) = DMA2D;
#endif
volatile GPIO_TypeDef         * const IO_GPIOA			__attribute__ ((section(".ioview"))) = GPIOA;
volatile GPIODATA_t           * const IO_GPIOA_ODR      __attribute__ ((section(".ioview"))) = (GPIODATA_t *)&GPIOA->ODR;
volatile GPIO_TypeDef         * const IO_GPIOB			__attribute__ ((section(".ioview"))) = GPIOB;
volatile GPIODATA_t           * const IO_GPIOB_ODR      __attribute__ ((section(".ioview"))) = (GPIODATA_t *)&GPIOB->ODR;
volatile GPIO_TypeDef         * const IO_GPIOC			__attribute__ ((section(".ioview"))) = GPIOC;
volatile GPIODATA_t           * const IO_GPIOC_ODR      __attribute__ ((section(".ioview"))) = (GPIODATA_t *)&GPIOC->ODR;
volatile GPIO_TypeDef         * const IO_GPIOD			__attribute__ ((section(".ioview"))) = GPIOD;
volatile GPIODATA_t           * const IO_GPIOD_ODR      __attribute__ ((section(".ioview"))) = (GPIODATA_t *)&GPIOD->ODR;
volatile GPIO_TypeDef         * const IO_GPIOE			__attribute__ ((section(".ioview"))) = GPIOE;
volatile GPIODATA_t           * const IO_GPIOE_ODR      __attribute__ ((section(".ioview"))) = (GPIODATA_t *)&GPIOE->ODR;
volatile GPIO_TypeDef         * const IO_GPIOF			__attribute__ ((section(".ioview"))) = GPIOF;
volatile GPIODATA_t           * const IO_GPIOF_ODR      __attribute__ ((section(".ioview"))) = (GPIODATA_t *)&GPIOF->ODR;
volatile GPIO_TypeDef         * const IO_GPIOG			__attribute__ ((section(".ioview"))) = GPIOG;
volatile GPIODATA_t           * const IO_GPIOG_ODR      __attribute__ ((section(".ioview"))) = (GPIODATA_t *)&GPIOG->ODR;
volatile GPIO_TypeDef         * const IO_GPIOH			__attribute__ ((section(".ioview"))) = GPIOH;
volatile GPIODATA_t           * const IO_GPIOH_ODR      __attribute__ ((section(".ioview"))) = (GPIODATA_t *)&GPIOH->ODR;
volatile GPIO_TypeDef         * const IO_GPIOI			__attribute__ ((section(".ioview"))) = GPIOI;
volatile GPIODATA_t           * const IO_GPIOI_ODR      __attribute__ ((section(".ioview"))) = (GPIODATA_t *)&GPIOI->ODR;
volatile HASH_TypeDef         * const IO_HASH			__attribute__ ((section(".ioview"))) = HASH;
volatile I2C_TypeDef          * const IO_I2C1			__attribute__ ((section(".ioview"))) = I2C1;
volatile I2C_TypeDef          * const IO_I2C2			__attribute__ ((section(".ioview"))) = I2C2;
volatile I2C_TypeDef          * const IO_I2C3			__attribute__ ((section(".ioview"))) = I2C3;
volatile IWDG_TypeDef         * const IO_IWDG			__attribute__ ((section(".ioview"))) = IWDG;
volatile PWR_TypeDef          * const IO_PWR			__attribute__ ((section(".ioview"))) = PWR;
volatile RCC_TypeDef          * const IO_RCC			__attribute__ ((section(".ioview"))) = RCC;
volatile RNG_TypeDef          * const IO_RNG			__attribute__ ((section(".ioview"))) = RNG;
volatile RTC_TypeDef          * const IO_RTC			__attribute__ ((section(".ioview"))) = RTC;
volatile SDIO_TypeDef         * const IO_SDIO			__attribute__ ((section(".ioview"))) = SDIO;
volatile SPI_TypeDef          * const IO_SPI1			__attribute__ ((section(".ioview"))) = SPI1;
volatile SPI_TypeDef          * const IO_SPI2			__attribute__ ((section(".ioview"))) = SPI2;
volatile SPI_TypeDef          * const IO_SPI3			__attribute__ ((section(".ioview"))) = SPI3;
volatile SPI_TypeDef          * const IO_SPI4			__attribute__ ((section(".ioview"))) = SPI4;
volatile SPI_TypeDef          * const IO_SPI5			__attribute__ ((section(".ioview"))) = SPI5;
volatile SPI_TypeDef          * const IO_SPI6			__attribute__ ((section(".ioview"))) = SPI6;
volatile SYSCFG_TypeDef       * const IO_SYSCFG			__attribute__ ((section(".ioview"))) = SYSCFG;
volatile TIM_TypeDef          * const IO_TIM1			__attribute__ ((section(".ioview"))) = TIM1;
volatile TIM_TypeDef          * const IO_TIM2			__attribute__ ((section(".ioview"))) = TIM2;
volatile TIM_TypeDef          * const IO_TIM3			__attribute__ ((section(".ioview"))) = TIM3;
volatile TIM_TypeDef          * const IO_TIM4			__attribute__ ((section(".ioview"))) = TIM4;
volatile TIM_TypeDef          * const IO_TIM5			__attribute__ ((section(".ioview"))) = TIM5;
volatile TIM_TypeDef          * const IO_TIM6			__attribute__ ((section(".ioview"))) = TIM6;
volatile TIM_TypeDef          * const IO_TIM7			__attribute__ ((section(".ioview"))) = TIM7;
volatile TIM_TypeDef          * const IO_TIM8			__attribute__ ((section(".ioview"))) = TIM8;
volatile TIM_TypeDef          * const IO_TIM9			__attribute__ ((section(".ioview"))) = TIM9;
volatile TIM_TypeDef          * const IO_TIM10			__attribute__ ((section(".ioview"))) = TIM10;
volatile TIM_TypeDef          * const IO_TIM11			__attribute__ ((section(".ioview"))) = TIM11;
volatile TIM_TypeDef          * const IO_TIM12			__attribute__ ((section(".ioview"))) = TIM12;
volatile TIM_TypeDef          * const IO_TIM13			__attribute__ ((section(".ioview"))) = TIM13;
volatile TIM_TypeDef          * const IO_TIM14			__attribute__ ((section(".ioview"))) = TIM14;
volatile USART_TypeDef        * const IO_USART1			__attribute__ ((section(".ioview"))) = USART1;
volatile USART_TypeDef        * const IO_USART2			__attribute__ ((section(".ioview"))) = USART2;
volatile USART_TypeDef        * const IO_USART3			__attribute__ ((section(".ioview"))) = USART3;
volatile USART_TypeDef        * const IO_UART4			__attribute__ ((section(".ioview"))) = UART4;
volatile USART_TypeDef        * const IO_UART5			__attribute__ ((section(".ioview"))) = UART5;
volatile USART_TypeDef        * const IO_USART6			__attribute__ ((section(".ioview"))) = USART6;
volatile WWDG_TypeDef         * const IO_WWDG			__attribute__ ((section(".ioview"))) = WWDG;

/* Variables -----------------------------------------------------------------*/

/* Prototypes ----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/* End Of File ---------------------------------------------------------------*/
