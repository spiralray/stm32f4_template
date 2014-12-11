#pragma once
#include <string.h>
#include "config/stm32plus.h"

class NMEA0183{
public:
  static uint8_t calcCheckSum(char *str){
    int length = strlen(str);

    uint8_t checksum = 0x00;
    for(int i=0;i<length;i++){
	checksum ^= str[i];
    }

    return checksum;
  }
};
