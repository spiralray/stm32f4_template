#include "utils/debug.h"

using namespace stm32plus;

#ifdef DEBUG_PORT
DebugStream<DEBUG_PORT<> > debug(115200);
#endif
