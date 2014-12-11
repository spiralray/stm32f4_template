#include "config/stm32plus.h"
#include "config/usart.h"
#include "debugstream.h"

#ifdef USE_MAIN_V2
  #define UsartDebug Usart1
#elif defined USE_MAIN_V3

#if MAIN_V3_DEBUG_FT232
  #define UsartDebug Usart2_Remap1
  #define DEBUG_BAUD 115200
#elif MAIN_V3_DEBUG_XBEE
  #define UsartDebug Usart1_Remap1
#else
  #error MAIN_V3 debug port not defined! Select MAIN_V3_DEBUG_FT232 or MAIN_V3_DEBUG_XBEE in Makefile.
#endif
#elif defined USE_STM32F4DISCOVERY
  #define UsartDebug Usart2
#elif !defined UsartDebug
  #error UsartDebug not defined! Select EVAL_BOARD or delete inclusion of "utils/debug.h"
#endif

#ifndef DEBUG_BAUD
  #define DEBUG_BAUD 115200
#endif

extern DebugStream<stm32plus::UsartDebug<> > debug;
