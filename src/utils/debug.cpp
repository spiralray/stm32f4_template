#include "utils/debug.h"

using namespace stm32plus;
#if defined UsartDebug
DebugStream<UsartDebug<> > debug(DEBUG_BAUD);
#endif
