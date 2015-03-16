#include "config/stm32plus.h"
#include "config/usart.h"
#include "debugstream.h"

#ifdef DEBUG_PORT
  extern DebugStream<stm32plus::DEBUG_PORT<> > debug;
#endif
