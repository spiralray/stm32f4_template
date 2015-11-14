/* General Inclusion */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#include "utils/debug.h"

using namespace stm32plus;

#ifdef DEBUG_PORT
DebugStream<DEBUG_PORT> debug(115200);

extern "C" void putch(uint8_t data)
{
	debug.write(data);
}

extern "C" uint8_t getch(void){
	if (debug.usart.dataAvailable() )	return debug.usart.receive();
	else								return false;

}

#endif
