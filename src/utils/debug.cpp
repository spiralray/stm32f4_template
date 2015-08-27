/* General Inclusion */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#include "utils/debug.h"

using namespace stm32plus;

#ifdef DEBUG_PORT
DebugStream<UsartInterruptWithBuffer<DEBUG_PORT> > debug(115200);

extern "C" void putch(uint8_t data)
{
	debug.write(data);
}

extern "C" uint8_t getch(void){
	if (debug.usart.RXBufferData_Available() )	return debug.usart.getch();
	else										return false;

}

#endif
