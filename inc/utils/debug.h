#include "config/stm32plus.h"
#include "config/usart.h"
#include "debugstream.h"

#ifdef DEBUG_PORT
  extern DebugStream<UsartInterruptWithBuffer<DEBUG_PORT> > debug;

#ifdef __cplusplus
 extern "C" {
#endif
  void putch(uint8_t data);
  uint8_t getch(void);
#ifdef __cplusplus
 }
#endif

#endif
