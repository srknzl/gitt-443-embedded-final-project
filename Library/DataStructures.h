#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "LPC407x_8x_177x_8x.h"

typedef struct {
	uint32_t distance; // in cms 
	uint16_t lightLevelLeft;
	uint16_t lightLevelRight;
	char* opmode; // AUTO or TEST
	uint32_t speed; // 0-100
} DeviceStatus;

	
#endif
